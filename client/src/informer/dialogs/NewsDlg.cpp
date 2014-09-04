//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/NewsDlg.h>
#include <informer/utility/BindDialogHandler.h>
#include <core/common.hpp>
#include <net/http/uri_builder.hpp>
#include <utility/file_utils.hpp>


// Event handlers

BOOL CNewsDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    DlgResize_Init(false, false);

    chat::factory::storage().on_news_updated(
        BindDialogHandler(&CNewsDlg::OnNewsUpdated));

    UpdateNews(chat::factory::storage().news());

    return TRUE;
}

void CNewsDlg::OnClose()
{
    DestroyWindow();
}

void CNewsDlg::OnDestroy()
{
    SetMsgHandled(FALSE);

    if(m_download)
    {
        m_download->abort();
        m_download.reset();
    }
}

// Storage event handlers

void CNewsDlg::OnNewsUpdated(chat::news const& news)
{
    UpdateNews(news);
}

// Downloader event handlers

void CNewsDlg::OnTemplateDownloaded(chat::download::pointer download, boost::system::error_code const& error)
{
    //if(IsWindow())
    //{
    if(!error)
        UpdateNews(chat::factory::storage().news());
    //}
}

// Implementation

void CNewsDlg::UpdateNews(chat::news const& news)
{
    boost::filesystem::path filename(L"data/news.html");

    if(!boost::filesystem::exists(filename))
    {
        net::http::uri_builder uri_builder;

        uri_builder.set_host(chat::informer_hostname);
        uri_builder.set_path(chat::news_template_path);

        //m_download = chat::download::create(uri_builder.uri(),
        //    filename, boost::bind(&CNewsDlg::OnTemplateDownloaded, this, _1));

        //m_download = chat::download::create(uri_builder.uri(), filename);
        //m_download->on_completion(BindDialogHandler(&CNewsDlg::OnTemplateDownloaded));

        //chat::factory::downloader().add_download(m_download);
    }
    else
    {
        std::string html_template = util::file::read(filename);

        std::string html_data = boost::algorithm::replace_first_copy(
            html_template, std::string("<!-- %NEWS% -->"), news.to_string());

        //m_ctrlHtml.LoadHtml(html_data.c_str());
    }
}
