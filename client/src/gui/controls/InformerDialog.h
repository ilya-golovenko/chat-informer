//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/utility/InformerHelper.h>


#define DIALOG_RESOURCE_ID(id)      static int const IDD = id;

#define IDD_INVALID                 -1


template
<
    class T,
    class TBase = CWindow
>
class ATL_NO_VTABLE CInformerDlgImpl :
    public CIdleHandler,
    public CMessageFilter,
    public CInformerHelper,
    public CDialogImpl<T, TBase>
{
public:
    enum
    {
        OFFSET = 10
    };

public:
    // Operations

    static std::shared_ptr<T> Create(HWND hWndParent)
    {
        std::shared_ptr<T> dialog = std::make_shared<T>();
        dialog->CDialogImpl<T, TBase>::Create(hWndParent);

        if(!dialog->IsWindow())
            throw std::runtime_error("cannot create dialog");

        return dialog;
    }

    void CenterWindow()
    {
        CRect rcWork;
        CRect rcWindow;

        GetWindowRect(rcWindow);
        SystemParametersInfo(SPI_GETWORKAREA, 0, rcWork, 0);

        int x = (rcWork.Width() - rcWindow.Width()) / 2;
        int y = (rcWork.Height() - rcWindow.Height()) / 2;

        SetWindowPos(NULL, x, y, 0, 0, SWP_NOACTIVATE
            | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
    }

    void RemoveSystemMenuItem(UINT itemID)
    {
        CMenuHandle systemMenu = GetSystemMenu(FALSE);
        systemMenu.DeleteMenu(itemID, MF_BYCOMMAND);
    }

    // Message map and event handlers

    BEGIN_MSG_MAP(CInformerDlgImpl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_NCDESTROY(OnNcDestroy)
        MSG_WM_SIZING(OnSizing)
        MSG_WM_WINDOWPOSCHANGING(OnWindowPosChanging)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CTLCOLORDLG(OnCtlColorDlg)
        MSG_WM_CTLCOLORSTATIC(OnCtlColorDlg)
        MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
        MSG_WM_CTLCOLORBTN(OnCtlColorEdit)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    BOOL OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
    {
        SetMsgHandled(FALSE);

        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);

        pLoop->AddIdleHandler(this);
        pLoop->AddMessageFilter(this);

        return TRUE;
    }

    void OnDestroy()
    {
        SetMsgHandled(FALSE);

        CMessageLoop* pLoop = _Module.GetMessageLoop();
        ATLASSERT(pLoop != NULL);

        pLoop->RemoveMessageFilter(this);
        pLoop->RemoveIdleHandler(this);
    }

    void OnNcDestroy()
    {
        SetMsgHandled(FALSE);
        m_dialogManager.DestroyDialog(static_cast<T*>(this)->IDD);
    }

    void OnSizing(UINT fwSide, LPRECT lpRect)
    {
        CRect rcWork;

        ::SystemParametersInfo(SPI_GETWORKAREA, 0, rcWork, 0);

        if(fwSide == WMSZ_LEFT || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_TOPLEFT)
        {
            if(OFFSET >= abs(lpRect->left - rcWork.left))
                lpRect->left = rcWork.left;
        }

        if(fwSide == WMSZ_RIGHT || fwSide == WMSZ_BOTTOMRIGHT || fwSide == WMSZ_TOPRIGHT)
        {
            if(OFFSET >= abs(lpRect->right - rcWork.right))
                lpRect->right = rcWork.right;
        }

        if(fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT)
        {
            if(OFFSET >= abs(lpRect->top - rcWork.top))
                lpRect->top = rcWork.top;
        }

        if(fwSide == WMSZ_BOTTOM || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_BOTTOMRIGHT)
        {
            if(OFFSET >= abs(lpRect->bottom - rcWork.bottom))
                lpRect->bottom = rcWork.bottom;
        }
    }

    void OnWindowPosChanging(LPWINDOWPOS lpWindowPos)
    {
        CRect rcWork;

        ::SystemParametersInfo(SPI_GETWORKAREA, 0, rcWork, 0);

        if(OFFSET >= abs(lpWindowPos->x - rcWork.left))
            lpWindowPos->x = rcWork.left;

        if(OFFSET >= abs(lpWindowPos->y - rcWork.top))
            lpWindowPos->y = rcWork.top;

        if(OFFSET >= abs(lpWindowPos->x + lpWindowPos->cx - rcWork.right))
            lpWindowPos->x = rcWork.right - lpWindowPos->cx;

        if(OFFSET >= abs(lpWindowPos->y + lpWindowPos->cy - rcWork.bottom))
            lpWindowPos->y = rcWork.bottom - lpWindowPos->cy;
    }

    void OnShowWindow(BOOL bShow, UINT /*nStatus*/)
    {
        SetMsgHandled(FALSE);

        if(bShow)
            EnsureVisible();
    }

    HBRUSH OnCtlColorDlg(CDCHandle dc, HWND /*hWnd*/)
    {
        dc.SetBkMode(OPAQUE);

        dc.SetBkColor(m_drawManager.Color(COLOR_BACK_DARK));
        dc.SetTextColor(m_drawManager.Color(COLOR_TEXT_NORMAL));

        return m_drawManager.Brush(BRUSH_DARK);
    }

    HBRUSH OnCtlColorEdit(CDCHandle dc, HWND /*hWnd*/)
    {
        dc.SetBkMode(OPAQUE);

        dc.SetBkColor(m_drawManager.Color(COLOR_BACK_LIGHT));
        dc.SetTextColor(m_drawManager.Color(COLOR_TEXT_NORMAL));

        return m_drawManager.Brush(BRUSH_LIGHT);
    }

    // CIdleHandler

    virtual BOOL OnIdle()
    {
        return FALSE;
    }

    // CMessageFilter

    virtual BOOL PreTranslateMessage(LPMSG lpMsg)
    {
        ATLASSERT(lpMsg != NULL);

        if(WM_KEYDOWN == lpMsg->message
            && VK_ESCAPE == lpMsg->wParam)
        {
            if(IsChild(lpMsg->hwnd))
            {
                PostMessage(WM_SYSCOMMAND, SC_CLOSE);
                return TRUE;
            }
        }

        return IsDialogMessage(lpMsg);
    }

    // Implementation

    void EnsureVisible()
    {
        CRect rcWnd;
        CRect rcWork;

        GetWindowRect(rcWnd);
        SystemParametersInfo(SPI_GETWORKAREA, 0, rcWork, 0);

        LONG cx = min(rcWnd.Width(), rcWork.Width());
        LONG cy = min(rcWnd.Height(), rcWork.Height());

        LONG x = max(rcWnd.left, rcWork.left);
        LONG y = max(rcWnd.top, rcWork.top);

        x = min(x, rcWork.right - cx);
        y = min(y, rcWork.bottom - cy);

        MoveWindow(x, y, cx, cy);
    }
};
