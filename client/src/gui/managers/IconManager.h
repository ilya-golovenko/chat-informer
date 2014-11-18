//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/resources/resource.h>
#include <informer/managers/ManagerBase.h>

// BOOST headers
#include <boost/filesystem/path.hpp>

// STL headers
#include <string>
#include <map>


//TODO: normal icon: alpha = 75%, hovered icon: alpha = 100%, disabled icon: grayscale
//TODO: load icon files only for the first time, then save ImageList with all icons and use it in future

// Icons
enum
{
    ICON_INVALID = -1,
    OVERLAY_NONE = -1,

    // Logo icons
    ICON_YELLOW,
    ICON_GREEN,
    ICON_RED,

    // User icons
    ICON_FEMALE,
    ICON_MALE,
    ICON_GUEST,
    ICON_HIDDEN,

    // Event icons
    ICON_FORUM,
    ICON_BIRTHDAY,
    ICON_NEWS,
    ICON_PHOTO,
    ICON_ALARM,

    // Menu icons
    ICON_WINDOW,
    ICON_INFO,
    ICON_LINKS,
    ICON_AUTH,
    ICON_LOGIN,
    ICON_OPTIONS,
    ICON_EXIT,
    ICON_MESSAGE,
    ICON_CHECK,
    ICON_ABOUT,
    ICON_FORUM_TOPIC,

    // Overlays
    OVERLAY_ADD,
    OVERLAY_REMOVE,
    OVERLAY_UP,
    OVERLAY_DOWN,
    OVERLAY_SEARCH,
    OVERLAY_VIEW,
    OVERLAY_NEW,
    OVERLAY_ATTENTION,
    OVERLAY_ERROR,

    ICON_COUNT
};

// Icon draw states
enum
{
    NORMAL_ICON,
    HOVERED_ICON,
    DISABLED_ICON
};

class CIconManager : public CManagerBase<CIconManager>
{
public:
    CIconManager();
    ~CIconManager();

    void Initialize() override;
    void Finalize() override;

    HICON GetIcon(int index);

    bool ReloadIcons();
    bool ReloadIcons(boost::filesystem::path const& path);

    int FindCustomIconIndex(std::wstring const& id);
    void AddCustomIcon(std::wstring const& id, boost::filesystem::path const& filename);

    void DrawIcon(HDC hDC, CPoint pt, int icon, int overlay, int state);
    void DrawIcon(HDC hDC, CPoint pt, std::wstring const& id, int overlay, int state);

private:
    void CreateImageList();
    void DestroyImageList();
    void RecreateImageList();

    void CreateBlankIcon();

    int LoadIconFromFile(boost::filesystem::path const& filename);

private:
    typedef std::map<std::wstring, int> index_map;

private:
    int m_cx;
    int m_cy;

    CIcon m_iconBlank;
    CImageList m_icons;

    index_map m_customIcons;
};
