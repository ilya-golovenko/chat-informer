//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/IconManager.h>
#include <informer/common/Logging.h>


// Icons file names
static wchar_t const* iconNames[ICON_COUNT] =
{
    // Logo icons
    L"logo_yellow.ico",         // ICON_YELLOW
    L"logo_green.ico",          // ICON_GREEN
    L"logo_red.ico",            // ICON_RED

    // User icons
    L"user_female.ico",         // ICON_FEMALE
    L"user_male.ico",           // ICON_MALE
    L"user_guest.ico",          // ICON_GUEST
    L"user_hidden.ico",         // ICON_HIDDEN

    // Event icons
    L"forum.ico",               // ICON_FORUM
    L"birthday.ico",            // ICON_BIRTHDAY
    L"news.ico",                // ICON_NEWS
    L"photo.ico",               // ICON_PHOTO
    L"alarm.ico",               // ICON_ALARM

    // Menu icons
    L"window.ico",              // ICON_WINDOW
    L"information.ico",         // ICON_INFO
    L"links.ico",               // ICON_LINKS
    L"authorization.ico",       // ICON_AUTH
    L"login.ico",               // ICON_LOGIN
    L"options.ico",             // ICON_OPTIONS
    L"exit.ico",                // ICON_EXIT
    L"message.ico",             // ICON_MESSAGE
    L"check.ico",               // ICON_CHECK
    L"about.ico",               // ICON_ABOUT
    L"forum_topic.ico",         // ICON_FORUM_TOPIC

    // Overlays
    L"overlay_add.ico",         // OVERLAY_ADD
    L"overlay_remove.ico",      // OVERLAY_REMOVE
    L"overlay_up.ico",          // OVERLAY_UP
    L"overlay_down.ico",        // OVERLAY_DOWN
    L"overlay_search.ico",      // OVERLAY_SEARCH
    L"overlay_view.ico",        // OVERLAY_VIEW
    L"overlay_new.ico",         // OVERLAY_NEW
    L"overlay_attention.ico",   // OVERLAY_ATTENTION
    L"overlay_error.ico"        // OVERLAY_ERROR
};

struct CBitmapInfo : BITMAPINFO
{
    CBitmapInfo()
    {
        ::ZeroMemory(this, sizeof(BITMAPINFO));
        bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    }
};

struct CImageListDrawParams : IMAGELISTDRAWPARAMS
{
    CImageListDrawParams(HIMAGELIST hImageList)
    {
        ::ZeroMemory(this, sizeof(IMAGELISTDRAWPARAMS));
        cbSize = sizeof(IMAGELISTDRAWPARAMS);
        fStyle = ILD_TRANSPARENT;
        himl = hImageList;
    }
};

CIconManager::CIconManager() :
    m_cx(::GetSystemMetrics(SM_CXSMICON)),
    m_cy(::GetSystemMetrics(SM_CYSMICON))
{
}

CIconManager::~CIconManager()
{
    DestroyImageList();
}

void CIconManager::Initialize()
{
    LOG_INFO("initializing");

    CreateBlankIcon();
    ReloadIcons();
}

void CIconManager::Finalize()
{
    LOG_INFO("finalizing");

    DestroyImageList();
}

HICON CIconManager::GetIcon(int icon)
{
    ATLASSERT(icon >= 0);
    ATLASSERT(!m_icons.IsNull());

    return m_icons.GetIcon(icon, ILD_TRANSPARENT);
}

bool CIconManager::ReloadIcons()
{
    return ReloadIcons("../skins/default/icons");
}

bool CIconManager::ReloadIcons(boost::filesystem::path const& path)
{
    LOG_DEBUG("reloading icons from path: ", path);

    RecreateImageList();

    for(int i = 0; i < ICON_COUNT; ++i)
    {
        boost::filesystem::path filename = path
            / boost::filesystem::path(iconNames[i]);

        if(LoadIconFromFile(filename) < 0)
            return false;
    }

    return true;
}

int CIconManager::FindCustomIconIndex(std::wstring const& id)
{
    ATLASSERT(!id.empty());
    ATLASSERT(!m_icons.IsNull());

    index_map::const_iterator it = m_customIcons.find(boost::to_lower_copy(id));

    if(it != m_customIcons.end())
        return it->second;

    return -1;
}

void CIconManager::AddCustomIcon(std::wstring const& id, boost::filesystem::path const& filename)
{
    ATLASSERT(!id.empty());
    ATLASSERT(!filename.empty());
    ATLASSERT(!m_icons.IsNull());

    LOG_DEBUG("adding custom icon (id: ", id, "; filename: ", filename, ")");

    m_customIcons.insert(index_map::value_type(boost::to_lower_copy(id), LoadIconFromFile(filename)));
}

void CIconManager::DrawIcon(HDC hDC, CPoint pt, int icon, int overlay, int state)
{
    ATLASSERT(icon >= 0);
    ATLASSERT(!m_icons.IsNull());

    CImageListDrawParams drawParams(m_icons);

    drawParams.hdcDst = hDC;
    drawParams.i = icon;
    drawParams.x = pt.x;
    drawParams.y = pt.y;

    if(DISABLED_ICON == state)
        drawParams.fState = ILS_SATURATE;

    m_icons.DrawIndirect(&drawParams);

    if(0 <= overlay)
    {
        drawParams.i = overlay;
        m_icons.DrawIndirect(&drawParams);
    }
}

void CIconManager::DrawIcon(HDC hDC, CPoint pt, std::wstring const& id, int overlay, int state)
{
    ATLASSERT(!id.empty());
    ATLASSERT(!m_icons.IsNull());

    int icon = FindCustomIconIndex(id);

    if(icon >= 0)
        DrawIcon(hDC, pt, icon, overlay, state);
}

void CIconManager::CreateImageList()
{
    ATLASSERT(m_icons.IsNull());

    LOG_DEBUG("creating image list");

    ATLVERIFY(m_icons.Create(m_cx, m_cy, ILC_MASK | ILC_COLOR32, ICON_COUNT, 8));
}

void CIconManager::DestroyImageList()
{
    if(!m_icons.IsNull())
    {
        LOG_DEBUG("destroying image list");

        m_icons.Destroy();
        m_customIcons.clear();
    }
}

void CIconManager::RecreateImageList()
{
    LOG_DEBUG("recreating image list");

    DestroyImageList();
    CreateImageList();
}

int CIconManager::LoadIconFromFile(boost::filesystem::path const& filename)
{
    ATLASSERT(!m_icons.IsNull());
    ATLASSERT(!filename.empty());

    CIcon icon;

    LOG_DEBUG("loading icon from file: ", filename);
    icon.LoadIcon(filename.c_str(), m_cx, m_cy, LR_LOADFROMFILE);

    if(icon.IsNull())
    {
        LOG_WARNING("cannot load icon, using blank");
        icon.Attach(m_iconBlank.DuplicateIcon());
    }

    if(!icon.IsNull())
        return m_icons.AddIcon(icon);

    LOG_ERROR("cannot use blank icon");

    return -1;
}

void CIconManager::CreateBlankIcon()
{
    if(m_iconBlank.IsNull())
    {
        LOG_DEBUG("creating blank icon");

        BYTE andMask[128];
        BYTE xorMask[128];

        std::memset(andMask, 0xFF, sizeof(andMask));
        std::memset(xorMask, 0x00, sizeof(xorMask));

        m_iconBlank.CreateIcon(m_cx, m_cy, 1, 1, andMask, xorMask);
    }
}
