//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>
#include <informer/managers/Credentials.h>
#include <informer/managers/InformerState.h>
#include <core/informer.hpp>
#include <core/download.hpp>
#include <core/events.hpp>

// MISSIO headers
#include <missio/json/json.hpp>


class CInformerManager :
    public CManagerBase<CInformerManager>
{
public:
    CInformerManager();

    virtual void Initialize();
    virtual void Finalize();

    bool IsOnline() const;
    bool IsStandby() const;
    bool IsOffline() const;

    InformerState GetState() const;

    template <typename Handler>
    void OnStateChanged(Handler const& handler);

    bool HasCredentials() const;
    bool IsBadCredentials() const;

    std::wstring GetNickName() const;
    std::wstring GetPassword() const;

    void UpdateCredentials(std::wstring const& nickname, std::wstring const& password);

private:
    //void OnUserIconDownloaded(std::wstring const& nickname, missio::download::completion_event_args const& args);
    //void OnFavoriteIconDownloaded(missio::download::completion_event_args const& args);
    void OnInformerQuery(missio::query::pointer query);
    void OnEventsUpdated(missio::event::type event);

    void UpdateInformerState(json::object_cref data);
    void CheckAuthorization(json::object_cref data);
    void UpdateFavoriteIcons(json::object_cref data);
    void UpdateUserIcons(json::object_cref data);

    void SetInformerState(InformerState state);
    void TerminateApplication(UINT stringID);
    void ShowNotification(UINT stringID, int iconID);

    void DownloadFavoriteIcon(net::http::uri const& uri, boost::filesystem::path const& filename);
    void AddFavoriteIcon(net::http::uri const& uri, boost::filesystem::path const& filename);

    void DownloadUserIcon(std::wstring const& nickname, boost::filesystem::path const& filename);
    void AddUserIcon(std::wstring const& nickname, boost::filesystem::path const& filename);

    void AddCustomIcon(std::wstring const& id, boost::filesystem::path const& filename);

private:
    boost::signals2::signal<void (InformerState)> m_state_changed;

private:
    InformerState m_state;
    CCredentials m_credentials;
};

template <typename Handler>
void CInformerManager::OnStateChanged(Handler const& handler)
{
    m_state_changed.connect(handler);
}
