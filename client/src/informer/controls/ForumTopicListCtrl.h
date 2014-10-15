//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// ATL/WTL headers
#include <atlwin.h>


#ifndef __cplusplus
    #error WTL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLWIN_H__
    #error ForumTopicListCtrl.h requires atlwin.h to be included first
#endif

#include <informer/resources/resource.h>
#include <informer/controls/InformerListCtrl.h>
#include <informer/controls/InformerMenu.h>
#include <informer/utility/ShellExecuteHelper.h>
#include <core/forum_topic_list.hpp>
#include <utility/time_to_string.hpp>


template
<
    class T,
    class TBase = CWindow,
    class TWinTraits = CControlWinTraits
>
class ATL_NO_VTABLE CForumTopicListCtrlImpl :
    public CInformerListCtrlImpl<T, TBase, TWinTraits>
{
public:
    enum
    {
        HEIGHT_MARGIN   = 6,
        TIME_OFFSET     = 8,
        TEXT_OFFSET     = 24
    };

public:
    static UINT const drawTimeFormat = DT_NOCLIP |
        DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE;

    static UINT const drawTextFormat = DT_NOCLIP |
        DT_LEFT | DT_EXPANDTABS | DT_NOPREFIX | DT_WORDBREAK;

public:
    struct ITEM_DATA
    {
        std::wstring topic_id;
        bool is_new;
        bool is_read;
    };

public:
    // Controls
    CInformerMenu m_topicMenu;

public:
    DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())

    // Operations

    void Assign(chat::forum_topic_list const& topics)
    {
        int curSel;
        UINT topIndex;
        std::wstring topicID;

        if(IsWindowVisible())
            topIndex = GetTopIndex();
        else
            topIndex = topics.size();

        curSel = GetCurSel();

        if(LB_ERR != curSel)
            topicID = m_topics[curSel].id();

        m_topics.assign(topics);

        if(LB_ERR != curSel)
        {
            std::size_t index = m_topics.index_of(topicID);

            if(chat::forum_topic_list::invalid_index != index)
                curSel = static_cast<int>(index);
        }

        BeginUpdate();
        SetCount(m_topics.size());
        SetCurSel(curSel);
        SetTopIndex(topIndex);
        EndUpdate();
    }

    void MarkTopicsAsOld()
    {
        m_topics.mark_as_old();
    }

    // Message map and handlers

    BEGIN_MSG_MAP(CForumTopicListCtrlImpl)
        MSG_WM_CONTEXTMENU(OnContextMenu)
        MSG_WM_COMMAND(OnCommand)
        MSG_OCM_COMMAND(OnCommand)
        CHAIN_MSG_MAP_MEMBER(m_topicMenu)
        CHAIN_MSG_MAP(CInformerListCtrlImpl)
    END_MSG_MAP()

    void OnContextMenu(HWND /*hWnd*/, CPoint pt)
    {
        if(pt.x < 0 || pt.y < 0)
        {
            int itemID = GetCurSel();

            if(LB_ERR != itemID)
            {
                CRect rc;
                GetItemRect(itemID, rc);
                ClientToScreen(rc);

                pt.x = rc.left;
                pt.y = rc.top - 2;
            }
            else
            {
                ::GetCursorPos(&pt);
            }
        }

        ShowPopupMenu(pt);
        SetMsgHandled(FALSE);
    }

    void OnCommand(UINT uNotifyCode, int nID, HWND /*hWnd*/)
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            if(0 == uNotifyCode)
            {
                switch(nID)
                {
                    case IDC_READTOPIC:
                        NavigateToTopicUri(itemID);
                        break;

                    case IDC_MARKASREAD:
                        MarkTopicAsRead(itemID);
                        break;

                    default:
                        SetMsgHandled(FALSE);
                }
            }
            else if(LBN_DBLCLK == uNotifyCode)
            {
                NavigateToTopicUri(itemID);
            }
        }
        else
        {
            SetMsgHandled(FALSE);
        }
    }

    // CInformerListCtrlImpl

    void MeasureItem(UINT itemID, CDCHandle dc, CSize& itemSize)
    {
        CRect rcClient;
        GetClientRect(rcClient);

        //if(m_timeTextWidth < 0)
        //    MeasureItemTimeTextWidth(m_topics[itemID], dc);

        int iconCX = ::GetSystemMetrics(SM_CXSMICON);
        //rcClient.left += TEXT_OFFSET + iconCX + m_timeTextWidth;

        itemSize.cy = MeasureItemHeight(m_topics[itemID], dc, rcClient);
    }

    void DrawItem(UINT itemID, CDCHandle dc, CRect& rcItem, int itemState)
    {
        int pen;
        int brush;
        int textColor;

        chat::forum_topic const& topic = m_topics[itemID];

        if(itemState & ITEM_SELECTED)
        {
            pen = PEN_SELECTED;
            brush = BRUSH_SELECTED;
        }
        else if(itemState & ITEM_HOVERED)
        {
            pen = PEN_INVALID;
            brush = BRUSH_HOVERED;
        }
        else
        {
            pen = PEN_INVALID;
            brush = BRUSH_LIGHT;
        }

        DrawRectangle(dc, rcItem, pen, brush);

        if(topic.is_new())
        {
            textColor = COLOR_TEXT_HOVERED;
        }
        else if(topic.is_read())
        {
            textColor = COLOR_TEXT_DISABLED;
        }
        else
        {
            if(itemState & ITEM_SELECTED)
                textColor = COLOR_TEXT_SELECTED;
            else
                textColor = COLOR_TEXT_NORMAL;
        }

        int iconCX = ::GetSystemMetrics(SM_CXSMICON);
        int iconCY = ::GetSystemMetrics(SM_CYSMICON);

        int overlay = topic.is_new() ? OVERLAY_NEW : OVERLAY_NONE;
        int iconState = topic.is_read() ? DISABLED_ICON : NORMAL_ICON;

        CPoint pt(2, rcItem.top + (rcItem.Height() - iconCY) / 2);
        m_iconManager.DrawIcon(dc, pt, ICON_FORUM_TOPIC, overlay, iconState);

        //std::time_t last_post_time = topic.last_post_time();
        //std::wstring time_text = util::time_to_string(last_post_time);

        //std::wstring topic_title = topic.title();

        CRect rcTime(rcItem.left + iconCX + TIME_OFFSET,
            rcItem.top, rcItem.right, rcItem.bottom);

        //CRect rcText(iconCX + m_timeTextWidth + TEXT_OFFSET,
        //    rcItem.top + 2, rcItem.right, rcItem.bottom);

        dc.SetBkMode(TRANSPARENT);
        dc.SelectFont(m_drawManager.Font(FONT_MAIN));
        dc.SetTextColor(m_drawManager.Color(textColor));

        //TODO: dc.DrawText(time_text.c_str(), time_text.size(), rcTime, drawTimeFormat);
        //TODO: dc.DrawText(topic_text.c_str(), topic_text.size(), rcText, drawTextFormat);
    }

    void DrawBackground(CDCHandle dc, CRect const& rcClient)
    {
        DrawFillRect(dc, rcClient, BRUSH_LIGHT);
    }

    // Implementation

    CForumTopicListCtrlImpl() :
        m_timeTextWidth(-1)
    {
    }

    void Init()
    {
        CInformerListCtrlImpl<T, TBase, TWinTraits>::Init();

        // Create post popup menu

        ATLVERIFY(m_topicMenu.LoadMenu(IDR_FORUMPOSTMENU));
        CInformerMenuHandle topicMenu = m_topicMenu.GetSubMenu(0);

        topicMenu.SetMenuItemImage(IDC_READTOPIC, FALSE, ICON_FORUM_TOPIC, OVERLAY_VIEW);
        topicMenu.SetMenuItemImage(IDC_MARKASREAD, FALSE, ICON_FORUM_TOPIC, OVERLAY_NEW);
        topicMenu.SetMenuDefaultItem(IDC_READTOPIC);
    }

    void ShowPopupMenu(CPoint pt)
    {
        int itemID = GetCurSel();

        if(LB_ERR != itemID)
        {
            chat::forum_topic const& topic = m_topics[itemID];

            BOOL enabled = topic.is_read() ? FALSE : TRUE;
            SetPostMenuItemEnabled(IDC_MARKASREAD, enabled);

            m_topicMenu.GetSubMenu(0).TrackPopupMenuEx(
                TPM_LEFTALIGN | TPM_BOTTOMALIGN,
                pt.x, pt.y, m_hWnd);
        }
    }

    void SetPostMenuItemEnabled(UINT itemID, BOOL enabled)
    {
        UINT enable = enabled ? MF_ENABLED : MF_DISABLED;
        m_topicMenu.GetSubMenu(0).EnableMenuItem(itemID, enable);
    }

    void MeasureItemTimeTextWidth(chat::forum_topic const& topic, CDCHandle dc)
    {
        CRect rcTime;

        CFontHandle newFont = m_drawManager.Font(FONT_MAIN);
        CFontHandle oldFont = dc.SelectFont(newFont);

        //std::wstring time_text = topic.time_text();

        //dc.DrawText(time_text.c_str(), time_text.size(),
        //    rcTime, drawTimeFormat | DT_CALCRECT);

        m_timeTextWidth = rcTime.Width();
        dc.SelectFont(oldFont);
    }

    int MeasureItemHeight(chat::forum_topic const& topic, CDCHandle dc, CRect& rcItem)
    {
        CFontHandle newFont = m_drawManager.Font(FONT_MAIN);
        CFontHandle oldFont = dc.SelectFont(newFont);

        //std::wstring topic_text = topic.topic_text();

        //dc.DrawText(topic_text.c_str(), topic_text.size(),
        //    rcItem, drawTextFormat | DT_CALCRECT);

        dc.SelectFont(oldFont);

        int iconCY = ::GetSystemMetrics(SM_CYSMICON);
        int height = std::max(rcItem.Height(), iconCY);

        return height + HEIGHT_MARGIN;
    }

    void NavigateToTopicUri(UINT itemID)
    {
        if(LB_ERR != itemID)
        {
            chat::forum_topic const& topic = m_topics[itemID];
            CShellExecuteHelper(topic.uri()).Start();
            MarkTopicAsRead(itemID);
        }
    }

    void MarkTopicAsRead(UINT itemID)
    {
        if(LB_ERR != itemID)
        {
            m_topics[itemID].mark_as_read();
            InvalidateItem(itemID);
        }
    }
/*
    static std::wstring BuildTopicText(chat::forum_topic const& topic)
    {
        using boost::spirit::karma;

        std::wostringstream text;

        text_ << topic.title() << L" (";

        if(post_count_ > 1)
            text << post_count_ << L" ";

        std::size_t count_mod = post_count_ % 10;

        if(post_count_ >= 5 && post_count_ <= 20
            || 0 == count_mod || count_mod >= 5)
        {
            text << L"сообщений от: ";
        }
        else if(1 == count_mod)
        {
            text << L"сообщение от: ";
        }
        else
        {
            text << L"сообщения от: ";
        }

        std::set<std::wstring> const& authors = topic.authors();
        text << karma::format(karma::string % L", ", authors);

        text << L")";
    }
*/
private:
    int m_itemTimeTextWidth;
    chat::forum_topic_list m_topics;
};

class CForumTopicListCtrl :
    public CForumTopicListCtrlImpl<CForumTopicListCtrl>
{
public:
    DECLARE_WND_SUPERCLASS(L"ForumTopicListCtrl", GetWndClassName())
};
