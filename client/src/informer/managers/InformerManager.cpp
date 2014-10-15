//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
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
#include <network/http/uri_builder.hpp>
#include <core/common.hpp>
#include <core/factory.hpp>
#include <core/server_errors.hpp>


CInformerManager::CInformerManager() :
    m_state(InformerStandby)
{
}

void CInformerManager::Initialize()
{
    LOG_COMP_TRACE_FUNCTION(CInformerManager);

    LOG_COMP_INFO(CInformerManager, "initializing");

    UpdateCredentials();

    chat::factory::informer().set_completion_handler(
        boost::bind(&CInformerManager::OnInformerQuery, this, _1));

    chat::factory::storage().on_events_updated(boost::bind(
        &CInformerManager::OnEventsUpdated, shared_from_this(), _1));

    chat::factory::informer().start();
    chat::factory::downloader().start();
}

void CInformerManager::Finalize()
{
    LOG_COMP_TRACE_FUNCTION(CInformerManager);

    LOG_COMP_INFO(CInformerManager, "finalizing");

    chat::factory::downloader().stop();
    chat::factory::informer().stop();
}

bool CInformerManager::IsOnline() const
{
    return InformerOnline == m_state;
}

bool CInformerManager::IsStandby() const
{
    return InformerStandby == m_state;
}

bool CInformerManager::IsOffline() const
{
    return InformerOffline == m_state;
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
    LOG_COMP_INFO(CInformerManager, "updating credentials");

    if(m_credentials.Update(nickname, password))
    {
        if(m_credentials.IsEmpty())
        {
            chat::factory::informer().reset_credentials();
        }
        else
        {
            chat::factory::informer().set_credentials(
                chat::credentials::create(nickname, password));
        }
    }
}

void CInformerManager::OnUserIconDownloaded(std::wstring const& nickname, chat::download::completion_event_args const& args)
{
    if(!args.error)
        AddUserIcon(nickname, args.filename);
}

void CInformerManager::OnFavoriteIconDownloaded(chat::download::completion_event_args const& args)
{
    if(!args.error)
        AddFavoriteIcon(args.download->uri(), args.filename);
}

void CInformerManager::OnInformerQuery(chat::query::pointer query)
{
    if(!query->error())
    {
        try
        {
            missio::json::object const& json_data = query->json_data();

            chat::factory::storage().update(json_data);

            UpdateInformerState(json_data);
            CheckAuthorization(json_data);
            UpdateFavoriteIcons(json_data);
            UpdateUserIcons(json_data);
        }
        catch(std::exception const& e)
        {
            LOG_COMP_ERROR(CInformerManager, "caught exception: ", e);
        }
    }
    else
    {
        switch(query->error())
        {
            case chat::error::network_io_error:
            case chat::error::bad_http_response:
                SetInformerState(InformerStandby);
                break;

            case chat::error::bad_server_data:
            case chat::error::bad_server_status:
                SetInformerState(InformerOffline);
                break;

            default:
                SetInformerState(InformerOnline);
        }
    }
}

void CInformerManager::OnEventsUpdated(chat::event::type event)
{
    CManagerFactory::Get<CEventManager>().AddEvent(event);
}

void CInformerManager::UpdateInformerState(missio::json::object const& json_data)
{
    if(json_data.contains("server_info"))
    {
        SetInformerState(InformerOnline);
    }
}

void CInformerManager::CheckAuthorization(missio::json::object const& json_data)
{
    if(json_data.contains("auth_result"))
    {
        missio::json::object const& json_result = json_data["auth_result"];

        if(json_result.contains("bad_credentials"))
        {
            cfg::chat::bad_credentials = true;
            ShowNotification(IDS_AUTH_CREDENTIALS, IDD_LOGIN);
        }

        if(!json_result["success"].get<bool>())
        {
            switch(json_result["error"].get<int>())
            {
                case chat::server_error::generic:
                case chat::server_error::db_connect:
                case chat::server_error::db_query:
                    TerminateApplication(IDS_AUTH_SERVER);
                    break;

                case chat::server_error::need_id:
                    TerminateApplication(IDS_AUTH_CLIENT);
                    break;

                case chat::server_error::id_banned:
                    TerminateApplication(IDS_AUTH_BANNED);
                    break;

                default:
                    TerminateApplication(IDS_AUTH_FAILURE);
            }
        }
    }
}

void CInformerManager::UpdateFavoriteIcons(missio::json::object const& json_data)
{
    if(json_data.contains("links"))
    {
        chat::link_list const& links = chat::factory::storage().links();

        for(std::size_t index = 0; index < links.size(); ++index)
        {
            chat::link::pointer link = links[index];

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

void CInformerManager::UpdateUserIcons(missio::json::object const& json_data)
{
    if(json_data.contains("icons"))
    {
        missio::json::array const& json_icons = json_data["icons"];

        for(missio::json::object const& json_icon : json_icons)
        {
            std::wstring const nickname = json_icon["nickname"];
            std::wstring const filename = json_icon["filename"];

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
        LOG_COMP_INFO(CInformerManager, "informer state has changed: ", state);

        m_state = state;
        m_state_changed(m_state);

        if(InformerOnline != m_state)
            chat::factory::storage().clear_users();
    }
}

void CInformerManager::TerminateApplication(UINT stringID)
{
    LOG_COMP_WARNING(CInformerManager, "terminating application");

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
    LOG_COMP_DEBUG(CInformerManager, "downloading favorite icon (uri: ", uri, "; filename: ", filename, ")");

    boost::filesystem::path path = "data/icons/links" / filename;

    net::uri::uri_builder uri_builder;

    uri_builder.set_host(uri.host());
    uri_builder.set_path("favicon.ico");

    chat::download::pointer download = chat::download::create(uri_builder.uri(),
        path, boost::bind(&CInformerManager::OnFavoriteIconDownloaded, this, _1));

    chat::factory::downloader().add_download(download);
}

void CInformerManager::AddFavoriteIcon(net::http::uri const& uri, boost::filesystem::path const& filename)
{
    AddCustomIcon(uri.host(), filename);
    chat::factory::storage().fire_links_updated();
}

void CInformerManager::DownloadUserIcon(std::wstring const& nickname, boost::filesystem::path const& filename)
{
    LOG_COMP_DEBUG(CInformerManager, "downloading user icon (nickname: ", nickname, "; filename: ", filename, ")");

    boost::filesystem::path path = "data/icons/users" / filename;

    net::http::uri_builder uri_builder;

    uri_builder.set_host(chat::informer_hostname);
    uri_builder.set_path(chat::informer_icon_path + filename.string());

    chat::download::pointer download = chat::download::create(uri_builder.uri(),
        path, boost::bind(&CInformerManager::OnUserIconDownloaded, this, nickname, _1));

    chat::factory::downloader().add_download(download);
}

void CInformerManager::AddUserIcon(std::wstring const& nickname, boost::filesystem::path const& filename)
{
    AddCustomIcon(nickname, filename);

    if(chat::factory::storage().users().contains(nickname))
        chat::factory::storage().update_users();
}

void CInformerManager::AddCustomIcon(std::wstring const& id, boost::filesystem::path const& filename)
{
    LOG_COMP_DEBUG(CInformerManager, "adding custom icon (id: ", id, "; filename: ", filename, ")");
    CManagerFactory::Get<CIconManager>().AddCustomIcon(id, filename);
}
