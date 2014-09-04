//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/PhotoAlbumDlg.h>
#include <informer/utility/BindDialogHandler.h>
#include <core/common.hpp>


// Event handlers

BOOL CPhotoAlbumDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    DlgResize_Init(false, false);

    CSize idealSize;
    m_ctrlPhotoAlbum.GetIdealSize(idealSize);
    ResizeClient(idealSize.cx, idealSize.cy);

    //TODO: at this point size is already overriden by ResizeClient function. FIX IT!
    SetupDialogSize();
    CenterWindow();

    chat::factory::storage().on_photoalbum_updated(
        BindDialogHandler(&CPhotoAlbumDlg::OnPhotoAlbumUpdated));

    //UpdatePhotoThumbnails(chat::factory::storage.photoalbum().fresh_photos());

    return TRUE;
}

void CPhotoAlbumDlg::OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo)
{
    lpMinMaxInfo->ptMinTrackSize.x = 160;
    lpMinMaxInfo->ptMinTrackSize.y = 140;
}

void CPhotoAlbumDlg::OnClose()
{
    DestroyWindow();
}

void CPhotoAlbumDlg::OnDestroy()
{
    SetMsgHandled(FALSE);
    m_downloads.cancel();
}

void CPhotoAlbumDlg::OnSize(UINT nType, CSize /*size*/)
{
    SetMsgHandled(FALSE);

    if(SIZE_MINIMIZED != nType)
    {
        CRect rcWindow;
        GetWindowRect(rcWindow);

        cfg::windows::photoalbum::width = rcWindow.Width();
        cfg::windows::photoalbum::height = rcWindow.Height();
    }
}

// Storage event handlers

void CPhotoAlbumDlg::OnPhotoAlbumUpdated(chat::photoalbum const& photoalbum)
{
    UpdatePhotoThumbnails(photoalbum.fresh_photos());
}

// Downloader event handlers
/*
void CPhotoAlbumDlg::OnPhotoThumbnailDownloaded(std::wstring const& photoID,
    chat::download::completion_event_args const& args)
{
    if(IsWindow())
    {
        m_downloads.remove(args.download);

        if(!args.error)
            m_ctrlPhotoAlbum.UpdatePhotoThumbnail(photoID);
    }
}
*/
// Implementation

void CPhotoAlbumDlg::UpdatePhotoThumbnails(chat::photo_list const& photos)
{
    m_ctrlPhotoAlbum.Assign(photos);

    for(std::size_t i = 0; i < photos.size(); ++i)
        DownloadPhotoThumbnail(photos[i]);
}

void CPhotoAlbumDlg::DownloadPhotoThumbnail(chat::photo const& photo)
{
    boost::filesystem::path filename;// = L"data/photos"
        // / boost::filesystem::path(photo.thumb());

    if(!boost::filesystem::exists(filename))
    {
        //TODO: chat::download::pointer download = chat::download::create(photo.get_thumb_uri(),
        //    filename, boost::bind(&CPhotoAlbumDlg::OnPhotoThumbnailDownloaded, this, photo.id(), _1));

        //chat::factory::downloader().add_download(download);
        //m_downloads.push_back(download);
    }
}

void CPhotoAlbumDlg::SetupDialogSize()
{
    SetWindowPos(NULL, 0, 0, cfg::windows::photoalbum::width, cfg::windows::photoalbum::height,
        SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}
