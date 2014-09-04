//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/controls/InformerDialog.h>
#include <informer/controls/InformerFrame.h>
#include <informer/controls/InformerButton.h>
#include <informer/controls/InformerThumbnail.h>
#include <core/factory.hpp>
#include <core/download.hpp>
#include <core/chat_user_info.hpp>


class CUserInfoDlg :
    public CInformerDlgImpl<CUserInfoDlg>,
    public CWinDataExchange<CUserInfoDlg>
{
public:
    // Controls
    CInformerFrame m_ctrlFrame;
    CInformerThumbnail m_ctrlThumbnail;
    CInformerButton m_btnOK;
    WTL::CHyperLink m_ctrlNickName;
    WTL::CHyperLink m_ctrlVkontakte;
    WTL::CHyperLink m_ctrlHomePage;
    WTL::CHyperLink m_ctrlEmail;
    WTL::CHyperLink m_ctrlIcq;

    // Members
    WTL::CString m_strFullName;
    WTL::CString m_strSex;
    WTL::CString m_strBirthDay;
    WTL::CString m_strBirthMonth;
    WTL::CString m_strBirthYear;
    WTL::CString m_strLocation;
    WTL::CString m_strPhone;
    WTL::CString m_strEyes;
    WTL::CString m_strHair;
    WTL::CString m_strHeight;
    WTL::CString m_strMeetings;
    WTL::CString m_strKnowsFrom;
    WTL::CString m_strRegTime;
    WTL::CString m_strLastJoin;
    WTL::CString m_strUserAbout;

public:
    DIALOG_RESOURCE_ID(IDD_USERINFO)

    // Operations

    void LoadUserInfo(WTL::CString const& strNickName);

    // Message map and event handlers

    BEGIN_MSG_MAP(CUserInfoDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_SETCURSOR(OnSetCursor)
        COMMAND_ID_HANDLER_EX(IDC_THUMBNAIL, OnThumbnail)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        CHAIN_MSG_MAP(CInformerDlgImpl<CUserInfoDlg>)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BEGIN_DDX_MAP(CUserInfoDlg)
        DDX_CONTROL(IDC_FRAME, m_ctrlFrame)
        DDX_CONTROL(IDC_NICKNAME, m_ctrlNickName)
        DDX_CONTROL(IDC_VKONTAKTE, m_ctrlVkontakte)
        DDX_CONTROL(IDC_HOMEPAGE, m_ctrlHomePage)
        DDX_CONTROL(IDC_EMAIL, m_ctrlEmail)
        DDX_CONTROL(IDC_ICQ, m_ctrlIcq)
        DDX_CONTROL(IDC_THUMBNAIL, m_ctrlThumbnail)
        DDX_CONTROL(IDOK, m_btnOK)
        DDX_TEXT(IDC_FULLNAME, m_strFullName)
        DDX_TEXT(IDC_SEX, m_strSex)
        DDX_TEXT(IDC_BIRTH_DAY, m_strBirthDay)
        DDX_TEXT(IDC_BIRTH_MONTH, m_strBirthMonth)
        DDX_TEXT(IDC_BIRTH_YEAR, m_strBirthYear)
        DDX_TEXT(IDC_LOCATION, m_strLocation)
        DDX_TEXT(IDC_PHONE, m_strPhone)
        DDX_TEXT(IDC_EYES, m_strEyes)
        DDX_TEXT(IDC_HAIR, m_strHair)
        DDX_TEXT(IDC_HEIGHT, m_strHeight)
        DDX_TEXT(IDC_MEETINGS, m_strMeetings)
        DDX_TEXT(IDC_KNOWSFROM, m_strKnowsFrom)
        DDX_TEXT(IDC_REGTIME, m_strRegTime)
        DDX_TEXT(IDC_LASTJOIN, m_strLastJoin)
        DDX_TEXT(IDC_USERABOUT, m_strUserAbout)
    END_DDX_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/);
    void OnClose();
    void OnDestroy();
    BOOL OnSetCursor(HWND hWnd, UINT /*nHitTest*/, UINT /*uMsg*/);
    void OnThumbnail(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnOK(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWnd*/);
    void OnUserInfo(chat::query::pointer query);

    // Downloader event handlers
    //void OnThumbnailDownloaded(chat::download::completion_event_args const& args);

    // Implementation
    CUserInfoDlg();
    void SetWindowCaption();
    void ResetDialogState();
    void AddUserInfoQuery();
    void SetupHyperLinks();
    void SetupHyperLink(WTL::CHyperLink& hyperLink, COLORREF clrText);
    void SetWindowCaption(std::wstring const& nickname, bool loading);
    void AssignNumericField(WTL::CString& string, int value);
    void AssignTimeField(WTL::CString& string, std::time_t time);
    void AssignSexField(WTL::CString& string, chat::chat_user::sex_type sex);
    void AssignStringField(WTL::CString& string, std::wstring const& data);
    void AssignHyperLinkField(WTL::CHyperLink& hyperLink, std::wstring const& field, std::wstring const& prefix);
    void FillUserData(chat::chat_user_info::pointer userInfo);
    void DownloadThumbnail(chat::photo const& photo);
    void CancelAllDownloads();

private:
    std::wstring m_nickname;
    chat::photo_list m_photos;
    std::size_t m_currentPhoto;
    chat::query::pointer m_query;
    chat::download::pointer m_download;
};
