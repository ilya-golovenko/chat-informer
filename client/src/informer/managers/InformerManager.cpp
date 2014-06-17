//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/InformerManager.h>
#include <informer/managers/DialogManager.h>
#include <informer/managers/EventManager.h>
#include <informer/managers/IconManager.h>
#include <informer/managers/SoundManager.h>
#include <informer/managers/ManagerFactory.h>
#include <informer/common/Config.h>
#include <informer/common/Logging.h>
#include <core/common.hpp>
#include <core/factory.hpp>
#include <core/server_errors.hpp>
#include <net/http/uri_builder.hpp>


CInformerManager::CInformerManager() :
    m_state(InformerStandby)
{
}

void CInformerManager::Initialize()
{
    LOG_INFO("initializing");

    UpdateCredentials();

    missio::factory::informer().set_completion_handler(
        boost::bind(&CInformerManager::OnInformerQuery, this, _1));

    missio::factory::storage().on_events_updated(boost::bind(
        &CInformerManager::OnEventsUpdated, shared_from_this(), _1));

    missio::factory::informer().start();
    missio::factory::downloader().start();
}

void CInformerManager::Finalize()
{
    LOG_INFO("finalizing");

    missio::factory::downloader().stop();
    missio::factory::informer().stop();
}

bool CInformerManager::IsOnline() const
{
    return (m_state == InformerOnline);
}

bool CInformerManager::IsStandby() const
{
    return (m_state == InformerStandby);
}

bool CInformerManager::IsOffline() const
{
    return (m_state == InformerOffline);
}

InformerState CInformerManager::GetState() const
{
    return m_state;
}

bool CInformerManager::HasCredentials() const
{
    return !m_credentials.IsEmpty();
}

bool CInformerManager::IsBadCredentials() const
{
    return m_credentials.IsBad();
}

std::wstring CInformerManager::GetNickName() const
{
    return m_credentials.GetNickName();
}

std::wstring CInformerManager::GetPassword() const
{
    return m_credentials.GetPassword();
}

void CInformerManager::UpdateCredentials(std::wstring const& nickname, std::wstring const& password);
{
    LOG_INFO("updating credentials");

    if(m_credentials.Update(nickname, password))
    {
        if(m_credentials.IsEmpty())
        {
            missio::factory::informer().reset_credentials();
        }
        else
        {
            missio::factory::informer().set_credentials(
                missio::credentials::create(nickname, password));
        }
    }
}

void CInformerManager::OnUserIconDownloaded(std::wstring const& nickname, missio::download::completion_event_args const& args)
{
    if(!args.error)
        AddUserIcon(nickname, args.filename);
}

void CInformerManager::OnFavoriteIconDownloaded(missio::download::completion_event_args const& args)
{
    if(!args.error)
        AddFavoriteIcon(args.download->uri(), args.filename);
}

void CInformerManager::OnInformerQuery(missio::query::pointer query)
{
    if(!query->error())
    {
        try
        {
            json::object_cref json_data = query->json_data();
            missio::factory::storage().update(json_data);

            UpdateInformerState(json_data);
            CheckAuthorization(json_data);
            UpdateFavoriteIcons(json_data);
            UpdateUserIcons(json_data);
        }
        catch(std::exception const& e)
        {
            LOG_ERROR("caught exception: ", e);
        }
    }
    else
    {
        switch(query->error())
        {
            case missio::error::network_io_error:
            case missio::error::bad_http_response:
                SetInformerState(InformerStandby);
                break;

            case missio::error::bad_server_data:
            case missio::error::bad_server_status:
                SetInformerState(InformerOffline);
                break;

            default:
                SetInformerState(InformerOnline);
        }
    }
}

void CInformerManager::OnEventsUpdated(missio::event::type event)
{
    CManagerFactory::Get<CEventManager>().AddEvent(event);
}

void CInformerManager::UpdateInformerState(json::object_cref json_data)
{
    if(json_data->contains("server_info"))
        SetInformerState(InformerOnline);
}

void CInformerManager::CheckAuthorization(json::object_cref json_data)
{
    if(json_data->contains("auth_result"))
    {
        json::object_cref json_result = json_data["auth_result"];

        if(json_result->contains("bad_credentials"))
        {
            cfg::chat::bad_credentials = true;
            ShowNotification(IDS_AUTH_CREDENTIALS, IDD_LOGIN);
        }

        if(!json_result["success"].as<bool>())
        {
            switch(json_result["error"].as<int>())
            {
                case missio::server_error::generic:
                case missio::server_error::db_connect:
                case missio::server_error::db_query:
                    TerminateApplication(IDS_AUTH_SERVER);
                    break;

                case missio::server_error::need_id:
                    TerminateApplication(IDS_AUTH_CLIENT);
                    break;

                case missio::server_error::id_banned:
                    TerminateApplication(IDS_AUTH_BANNED);
                    break;

                default:
                    TerminateApplication(IDS_AUTH_FAILURE);
            }
        }
    }
}

void CInformerManager::UpdateFavoriteIcons(json::object_cref json_data)
{
    if(json_data->contains("links"))
    {
        missio::link_list const& links = missio::factory::storage().links();

        for(std::size_t index = 0; index < links.size(); ++index)
        {
            missio::link::pointer link = links[index];

            net::http::uri uri = link->uri();
            std::wstring id = link->id();

            boost::filesystem::path filename("data/icons/links");
            filename /= boost::filesystem::path(id);
            filename.replace_extension(".ico");

            if(boost::filesystem::exists(filename))
                DownloadFavoriteIcon(uri, filename);
            else
                AddFavoriteIcon(uri, filename);
        }
    }
}

void CInformerManager::UpdateUserIcons(json::object_cref json_data)
{
    if(json_data->contains("icons"))
    {
        json::array_cref json_icons = json_data["icons"];

        for(std::size_t index = 0; index < json_icons->size(); ++index)
        {
            json::object_cref json_icon = json_icons[index];

            std::wstring nickname = json_icon["nickname"];
            std::wstring filename = json_icon["filename"];

            boost::filesystem::path path("data/icons/users");
            path /= boost::filesystem::path(filename);

            if(!boost::filesystem::exists(path))
                DownloadUserIcon(nickname, filename);
            else
                AddUserIcon(nickname, path);
        }
    }
}

void CInformerManager::SetInformerState(InformerState state)
{
    if(m_state != state)
    {
        LOG_INFO("informer state has changed: ", state);

        m_state = state;
        m_state_changed(m_state);

        if(InformerOnline != m_state)
            missio::factory::storage().clear_users();
    }
}

void CInformerManager::TerminateApplication(UINT stringID)
{
    LOG_WARNING("terminating application");

    CManagerFactory::Get<CSoundManager>().Play(SOUND_ERROR);
    CManagerFactory::Get<CDialogManager>().ShowMessage(stringID);
    CManagerFactory::Get<CDialogManager>().DestroyMainDialog();
}

void CInformerManager::ShowNotification(UINT stringID, int dialogID)
{
    CManagerFactory::Get<CDialogManager>().ShowNotification(stringID, dialogID);
}

void CInformerManager::DownloadFavoriteIcon(net::http::uri const& uri, boost::filesystem::path const& filename)
{
    LOG_DEBUG("downloading favorite icon (uri: ", uri, "; filename: ", filename, ")");

    boost::filesystem::path path = "data/icons/links" / filename;

    net::uri::uri_builder uri_builder;

    uri_builder.set_host(uri.host());
    uri_builder.set_path("favicon.ico");

    missio::download::pointer download = missio::download::create(uri_builder.uri(),
        path, boost::bind(&CInformerManager::OnFavoriteIconDownloaded, this, _1));

    missio::factory::downloader().add_download(download);
}

void CInformerManager::AddFavoriteIcon(net::http::uri const& uri, boost::filesystem::path const& filename)
{
    AddCustomIcon(uri.host(), filename);
    missio::factory::storage().fire_links_updated();
}

void CInformerManager::DownloadUserIcon(std::wstring const& nickname, boost::filesystem::path const& filename)
{
    LOG_DEBUG("downloading user icon (nickname: ", nickname, "; filename: ", filename, ")");

    boost::filesystem::path path = "data/icons/users" / filename;

    net::http::uri_builder uri_builder;

    uri_builder.set_host(missio::informer_hostname);
    uri_builder.set_path(missio::informer_icon_path + filename.string());

    missio::download::pointer download = missio::download::create(uri_builder.uri(),
        path, boost::bind(&CInformerManager::OnUserIconDownloaded, this, nickname, _1));

    missio::factory::downloader().add_download(download);
}

void CInformerManager::AddUserIcon(std::wstring const& nickname, boost::filesystem::path const& filename)
{
    AddCustomIcon(nickname, filename);

    if(missio::factory::storage().users().contains(nickname))
        missio::factory::storage().update_users();
}

void CInformerManager::AddCustomIcon(std::wstring const& id, boost::filesystem::path const& filename)
{
    LOG_DEBUG("adding custom icon (id: ", id, "; filename: ", filename, ")");
    CManagerFactory::Get<CIconManager>().AddCustomIcon(id, filename);
}
