//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/dialogs/UserInfoDlg.h>
#include <informer/utility/ShellExecuteHelper.h>
#include <core/common.hpp>
#include <core/photo.hpp>


// Operations

void CUserInfoDlg::LoadUserInfo(CString const& strNickName)
{
    std::wstring nickname = strNickName;

    if(!boost::algorithm::iequals(m_nickname, nickname))
    {
        SetWindowCaption(nickname, true);
        m_nickname = nickname;

        CancelAllDownloads();
        ResetDialogState();
        AddUserInfoQuery();

        //TODO: arrrrggghhh.....
        //m_currentPhoto = missio::photo_list::invalid_index;
        //m_photos.clear();
    }

    if(IsIconic())
        ShowWindow(SW_RESTORE);
}

// Event handlers

BOOL CUserInfoDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
    SetMsgHandled(FALSE);
    DoDataExchange(DDX_LOAD);
    SetupHyperLinks();

    return TRUE;
}

void CUserInfoDlg::OnClose()
{
    DestroyWindow();
}

void CUserInfoDlg::OnDestroy()
{
    SetMsgHandled(FALSE);
    CancelAllDownloads();
}

BOOL CUserInfoDlg::OnSetCursor(HWND hWnd, UINT /*nHitTest*/, UINT /*uMsg*/)
{
    if(hWnd == m_ctrlThumbnail)
    {
        if(missio::photo_list::invalid_index != m_currentPhoto)
        {
            SetCursor(AtlLoadSysCursor(IDC_HAND));
            return TRUE;
        }
    }

    SetMsgHandled(FALSE);
    return FALSE;
}

void CUserInfoDlg::OnThumbnail(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    if(missio::photo_list::invalid_index != m_currentPhoto)
    {
        missio::photo const& photo = m_photos[m_currentPhoto];
        CShellExecuteHelper(photo.get_photo_uri()).Start();
    }
}

void CUserInfoDlg::OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/)
{
    DestroyWindow();
}

// Informer event handlers

void CUserInfoDlg::OnUserInfo(missio::query::pointer query)
{
    if(IsWindow())
    {
        if(!query->error())
        {
            json::object_cref json_data = query->json_data();
            json::object_cref json_info = json_data["user_info"];

            if(missio::chat_user_info::contains_user_info(json_info))
            {
                FillUserData(missio::chat_user_info::create(json_info));
            }
            else
            {
                m_dialogManager.ShowMessage(IDS_NOT_REGISTERED);
                DestroyWindow();
            }
        }
        else
        {
            m_dialogManager.ShowMessage(IDS_LOAD_FAILURE);
            DestroyWindow();
        }
    }
}

// Downloader event handlers
/*
void CUserInfoDlg::OnThumbnailDownloaded(missio::download::completion_event_args const& args)
{
    if(IsWindow())
    {
        if(!args.error)
        {
            if(!m_ctrlThumbnail.LoadImageFromFile(args.filename.c_str()))
                m_ctrlThumbnail.LoadImageFromFile(L"skins/default/images/photonone.gif");
                //TODO: m_ctrlThumbnail.LoadImageFromFile(m_skinManager.GetImageFileName(IMAGE_PHOTO_NONE));
        }
        else if(args.error != boost::asio::error::operation_aborted)
        {
            m_ctrlThumbnail.LoadImageFromFile(L"skins/default/images/photonone.gif");
            //TODO: m_ctrlThumbnail.LoadImageFromFile(m_skinManager.GetImageFileName(IMAGE_PHOTO_NONE));
        }
    }
}
*/
// Implementation

CUserInfoDlg::CUserInfoDlg() :
    m_currentPhoto(missio::photo_list::invalid_index)
{
}

void CUserInfoDlg::SetWindowCaption(std::wstring const& nickname, bool loading)
{
    CString strCaption;

    if(loading)
        strCaption.Format(IDS_LOADING, nickname.c_str());
    else
        strCaption.Format(IDS_USER_PROFILE, nickname.c_str());

    SetWindowText(strCaption);
}

void CUserInfoDlg::ResetDialogState()
{
    m_strFullName.Empty();
    m_strSex.Empty();
    m_strBirthDay.Empty();
    m_strBirthMonth.Empty();
    m_strBirthYear.Empty();
    m_strLocation.Empty();
    m_strPhone.Empty();
    m_strEyes.Empty();
    m_strHair.Empty();
    m_strHeight.Empty();
    m_strMeetings.Empty();
    m_strKnowsFrom.Empty();
    m_strRegTime.Empty();
    m_strLastJoin.Empty();
    m_strUserAbout.Empty();

    m_ctrlNickName.ShowWindow(SW_HIDE);
    m_ctrlVkontakte.ShowWindow(SW_HIDE);
    m_ctrlHomePage.ShowWindow(SW_HIDE);
    m_ctrlEmail.ShowWindow(SW_HIDE);
    m_ctrlIcq.ShowWindow(SW_HIDE);

    DoDataExchange();
}

void CUserInfoDlg::AddUserInfoQuery()
{
    m_ctrlThumbnail.LoadImageFromFile(L"skins/default/images/photoloading.gif");
    //TODO: m_ctrlThumbnail.LoadImageFromFile(m_skinManager.GetImageFileName(IMAGE_PHOTO_LOADING));

    json::value json_data;

    json_data["user"]["nickname"] = m_nickname;

    m_query = missio::query::create(json_data, false,
        boost::bind(&CUserInfoDlg::OnUserInfo, this, _1));

    missio::factory::informer().add_query(m_query);
}

void CUserInfoDlg::SetupHyperLinks()
{
    COLORREF clrText = m_drawManager.Color(COLOR_TEXT_NORMAL);

    SetupHyperLink(m_ctrlNickName, clrText);
    SetupHyperLink(m_ctrlVkontakte, clrText);
    SetupHyperLink(m_ctrlHomePage, clrText);
    SetupHyperLink(m_ctrlEmail, clrText);
    SetupHyperLink(m_ctrlIcq, clrText);
}

void CUserInfoDlg::SetupHyperLink(CHyperLink& hyperLink, COLORREF clrText)
{
    hyperLink.m_clrLink = clrText;
    hyperLink.m_clrVisited = clrText;

    hyperLink.SetHyperLinkExtendedStyle(HLINK_NOTOOLTIP);
}

void CUserInfoDlg::AssignNumericField(CString& string, int value)
{
    if(value)
        string.Format(L"%d", value);
}

void CUserInfoDlg::AssignTimeField(CString& string, std::time_t time)
{
    if(!time)
        string.LoadString(IDS_UNKNOWN);
    else
        string = util::time_to_string(time).c_str();
}

void CUserInfoDlg::AssignSexField(CString& string, missio::chat_user::sex_type sex)
{
    switch(sex)
    {
        case missio::chat_user::hidden:
            string.LoadString(IDS_SEX_HIDDEN);
            break;

        case missio::chat_user::male:
            string.LoadString(IDS_SEX_MALE);
            break;

        case missio::chat_user::female:
            string.LoadString(IDS_SEX_FEMALE);
            break;

        default:
            string.LoadString(IDS_UNKNOWN);
    }
}

void CUserInfoDlg::AssignStringField(CString& string, std::wstring const& data)
{
    string = data.c_str();
}

void CUserInfoDlg::AssignHyperLinkField(CHyperLink& hyperLink, std::wstring const& value, std::wstring const& uri)
{
    if(value.empty() || uri.empty())
    {
        hyperLink.ShowWindow(SW_HIDE);
    }
    else
    {
        hyperLink.SetLabel(value.c_str());
        hyperLink.SetHyperLink(uri.c_str());
        hyperLink.ShowWindow(SW_SHOW);
    }
}

void CUserInfoDlg::FillUserData(missio::chat_user_info::pointer userInfo)
{
    SetWindowCaption(userInfo->nickname(), false);

    AssignStringField(m_strFullName, userInfo->fullname());
    AssignStringField(m_strBirthMonth, userInfo->birth_month());
    AssignStringField(m_strLocation, userInfo->location());
    AssignStringField(m_strPhone, userInfo->phone());
    AssignStringField(m_strEyes, userInfo->eyes());
    AssignStringField(m_strHair, userInfo->hair());
    AssignStringField(m_strHeight, userInfo->height());
    AssignStringField(m_strMeetings, userInfo->meetings());
    AssignStringField(m_strKnowsFrom, userInfo->knows_from());
    AssignStringField(m_strUserAbout, userInfo->about());

    AssignNumericField(m_strBirthDay, userInfo->birth_day());
    AssignNumericField(m_strBirthYear, userInfo->birth_year());

    AssignSexField(m_strSex, userInfo->sex());

    AssignTimeField(m_strRegTime, userInfo->reg_time());
    AssignTimeField(m_strLastJoin, userInfo->last_join());

    AssignHyperLinkField(m_ctrlNickName, userInfo->nickname(), userInfo->get_user_info_uri());
    AssignHyperLinkField(m_ctrlVkontakte, userInfo->vkontakte(), userInfo->get_vkontakte_uri());
    AssignHyperLinkField(m_ctrlHomePage, userInfo->homepage(), userInfo->homepage());
    AssignHyperLinkField(m_ctrlEmail, userInfo->email(), userInfo->get_email_uri());
    AssignHyperLinkField(m_ctrlIcq, userInfo->icq(), userInfo->get_icq_uri());

    DoDataExchange();

    m_photos = userInfo->photos();

    if(!m_photos.empty())
    {
        m_currentPhoto = 0;

        boost::filesystem::path filename;// = L"data/photos"
            // / boost::filesystem::path(m_photos[m_currentPhoto]->thumb());

        if(boost::filesystem::exists(filename))
        {
            if(!m_ctrlThumbnail.LoadImageFromFile(filename.c_str()))
                m_ctrlThumbnail.LoadImageFromFile(L"skins/default/images/photonone.gif");
                //TODO: m_ctrlThumbnail.LoadImageFromFile(m_skinManager.GetImageFileName(IMAGE_PHOTO_NONE));
        }
        else
        {
            DownloadThumbnail(m_photos[m_currentPhoto]);
        }
    }
    else
    {
        m_ctrlThumbnail.LoadImageFromFile(L"skins/default/images/photonone.gif");
        //TODO: m_ctrlThumbnail.LoadImageFromFile(m_skinManager.GetImageFileName(IMAGE_PHOTO_NONE));
    }
}

void CUserInfoDlg::DownloadThumbnail(missio::photo const& photo)
{
    boost::filesystem::path filename; //= L"data/photos"
        // / boost::filesystem::path(photo.thumb());

    //m_download = missio::download::create(photo.get_thumb_uri(), filename,
    //    boost::bind(&CUserInfoDlg::OnThumbnailDownloaded, this, _1));

    missio::factory::downloader().add_download(m_download);
}

void CUserInfoDlg::CancelAllDownloads()
{
    if(m_query)
    {
        m_query->cancel();
        m_query.reset();
    }

    if(m_download)
    {
        m_download->abort();
        m_download.reset();
    }
}
