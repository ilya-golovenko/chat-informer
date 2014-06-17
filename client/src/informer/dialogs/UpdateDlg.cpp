//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/UpdateDlg.h>
#include <informer/dialogs/MainDlg.h>
#include <informer/utility/ShellExecuteHelper.h>
#include <core/common.hpp>
#include <net/http/uri_builder.hpp>
#include <utility/file_utils.hpp>


// Event handlers

BOOL CUpdateDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);

    missio::storage& storage = missio::factory::storage();
    missio::version const& version = storage.server_version(); 

    m_strMessage.Format(IDS_UPDATE_READY, version.to_string().c_str());

    DoDataExchange(DDX_LOAD);
    m_ctrlProgress.SetRange(0, 100);

    return TRUE;
}

void CUpdateDlg::OnClose()
{
    DestroyWindow();
}

void CUpdateDlg::OnDestroy()
{
    SetMsgHandled(FALSE);
    CancelSetupDownload();
}

void CUpdateDlg::OnStartUpdate(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    SetMessage(IDS_DOWNLOADING);

    m_ctrlProgress.SetPos(0);
    m_ctrlProgress.ShowWindow(SW_SHOW);
    m_btnStartUpdate.EnableWindow(FALSE);

    StartDownloadingSetupExecutable();
}

void CUpdateDlg::OnCancel(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

// Downloader events
/*
void CUpdateDlg::OnSetupDownloadProgress(missio::download::progress_event_args const& args)
{
    if(IsWindow())
    {
        if(args.percent_completed >= 0)
            m_ctrlProgress.SetPos(args.percent_completed);
    }
}

void CUpdateDlg::OnSetupDownloadCompleted(missio::download::completion_event_args const& args)
{
    if(IsWindow())
    {
        if(args.error)
            SetApplicationUpdateFailure();
        else
            RunSetupExecutable(args.filename);
    }
}
*/
// Implementation

void CUpdateDlg::StartDownloadingSetupExecutable()
{
    missio::storage& storage = missio::factory::storage();
    missio::version const& version = storage.server_version(); 

    if(missio::version::unknown != version)
    {
        std::string filename;

        format::print(filename, missio::informer_setup_file,
            version.major(), version.minor(), version.build());

        boost::filesystem::path path = boost::filesystem::temp_directory_path() / filename;

        net::http::uri_builder uri_builder;

        uri_builder.set_host(missio::informer_hostname);
        uri_builder.set_path(missio::informer_setup_path + filename);
/*
        m_setup_download = missio::download::create(uri_builder.uri(),
            path, boost::bind(&CUpdateDlg::OnSetupDownloadProgress, this, _1),
            boost::bind(&CUpdateDlg::OnSetupDownloadCompleted, this, _1));
*/
        missio::factory::downloader().add_download(m_setup_download);
    }
    else
    {
        SetApplicationUpdateFailure();
    }
}

void CUpdateDlg::RunSetupExecutable(boost::filesystem::path const& filename)
{
    SetMessage(IDS_UPDATING);

    std::string parameters;

    boost::filesystem::path directory = boost::filesystem::initial_path();
    format::print(parameters, missio::informer_setup_params, directory);

    try
    {
        // Execute downloaded setup file
        //CShellExecuteHelper(filename, directory, parameters).Start();

        // Ensures that temporary file will be removed after reboot (note, that this function
        // requires Administrator privileges! Otherwise, the file will remain in temp folder)
        ::MoveFileEx(filename.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

        // Close main application window
        m_dialogManager.DestroyMainDialog();
    }
    catch(std::exception const& e)
    {
        SetApplicationUpdateFailure();
        m_dialogManager.ShowMessage(e.what());
    }
}

void CUpdateDlg::CancelSetupDownload()
{
    if(m_setup_download)
    {
        m_setup_download->abort();
        m_setup_download.reset();
    }
}

void CUpdateDlg::SetMessage(UINT nResourceID)
{
    m_strMessage.LoadString(nResourceID);
    DoDataExchange(DDX_LOAD, IDC_MESSAGE);
}

void CUpdateDlg::SetApplicationUpdateFailure()
{
    SetMessage(IDS_UPDATE_FAILURE);
    m_ctrlProgress.ShowWindow(SW_HIDE);
    m_btnStartUpdate.EnableWindow(TRUE);
}
