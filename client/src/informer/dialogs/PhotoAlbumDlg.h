//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/resources/resource.h>
#include <informer/controls/InformerDialog.h>
#include <informer/controls/PhotoAlbumCtrl.h>
#include <core/factory.hpp>
#include <core/download.hpp>
#include <core/photoalbum.hpp>
#include <net/http/download_queue.hpp>


class CPhotoAlbumDlg :
    public CInformerDlgImpl<CPhotoAlbumDlg>,
    public CDialogResize<CPhotoAlbumDlg>,
    public CWinDataExchange<CPhotoAlbumDlg>
{
public:
    // Controls
    CPhotoAlbumCtrl m_ctrlPhotoAlbum;

public:
    DIALOG_RESOURCE_ID(IDD_PHOTOALBUM)

    // Message map and event handlers

    BEGIN_MSG_MAP(CPhotoAlbumDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SIZE(OnSize)
        CHAIN_MSG_MAP(CInformerDlgImpl<CPhotoAlbumDlg>)
        CHAIN_MSG_MAP(CDialogResize<CPhotoAlbumDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DLGRESIZE_MAP(CPhotoAlbumDlg)
        DLGRESIZE_CONTROL(IDC_PHOTOALBUM, DLSZ_SIZE_X | DLSZ_SIZE_Y)
    END_DLGRESIZE_MAP()

    BEGIN_DDX_MAP(CPhotoAlbumDlg)
        DDX_CONTROL(IDC_PHOTOALBUM, m_ctrlPhotoAlbum)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
    void OnClose();
    void OnDestroy();
    void OnSize(UINT /*nType*/, CSize /*size*/);

    // Storage event handlers
    void OnPhotoAlbumUpdated(missio::photoalbum const& photoalbum);

    // Downloader event handlers
    //void OnPhotoThumbnailDownloaded(std::wstring const& photoID,
    //    missio::download::completion_event_args const& args);

    // Implementation
    void UpdatePhotoThumbnails(missio::photo_list const& photos);
    void DownloadPhotoThumbnail(missio::photo const& photo);
    void SetupDialogSize();

private:
    net::http::download_queue m_downloads;
};
