//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/DrawManager.h>
#include <informer/common/Logging.h>
#include <informer/common/Config.h>


CDrawManager::CDrawManager()
{
}

void CDrawManager::Initialize()
{
    LOG_INFO("initializing");

    CreateColors();
    CreateBrushes();
    CreatePens();
    CreateFonts();
}

void CDrawManager::Finalize()
{
    LOG_INFO("finalizing");

    DestroyBrushes();
    DestroyPens();
    DestroyFonts();
}

COLORREF CDrawManager::Color(std::size_t color)
{
    return m_colors[color];
}

HBRUSH CDrawManager::Brush(std::size_t brush)
{
    return static_cast<HBRUSH>(m_brushes[brush]);
}

HPEN CDrawManager::Pen(std::size_t pen)
{
    return static_cast<HPEN>(m_pens[pen]);
}

HFONT CDrawManager::Font(std::size_t font)
{
    return static_cast<HFONT>(m_fonts[font]);
}

void CDrawManager::CreateColors()
{
    LOG_DEBUG("creating colors");

    m_colors[COLOR_BACK_WHITE] = cfg::skin::color::back::white;
    m_colors[COLOR_BACK_LIGHT] = cfg::skin::color::back::light;
    m_colors[COLOR_BACK_DARK] = cfg::skin::color::back::dark;
    m_colors[COLOR_BACK_SELECTED] = cfg::skin::color::back::selected;
    m_colors[COLOR_BACK_HOVERED] = cfg::skin::color::back::hovered;

    m_colors[COLOR_RECT_NORMAL] = cfg::skin::color::rect::normal;
    m_colors[COLOR_RECT_SELECTED] = cfg::skin::color::rect::selected;
    m_colors[COLOR_RECT_HOVERED] = cfg::skin::color::rect::hovered;
    m_colors[COLOR_RECT_DISABLED] = cfg::skin::color::rect::disabled;
    m_colors[COLOR_RECT_STATIC] = cfg::skin::color::rect::static_;

    m_colors[COLOR_TEXT_NORMAL] = cfg::skin::color::text::normal;
    m_colors[COLOR_TEXT_SELECTED] = cfg::skin::color::text::selected;
    m_colors[COLOR_TEXT_HOVERED] = cfg::skin::color::text::hovered;
    m_colors[COLOR_TEXT_DISABLED] = cfg::skin::color::text::disabled;
}

void CDrawManager::CreateBrushes()
{
    LOG_DEBUG("creating brushes");

    m_brushes[BRUSH_WHITE].CreateSolidBrush(m_colors[COLOR_BACK_WHITE]);
    m_brushes[BRUSH_LIGHT].CreateSolidBrush(m_colors[COLOR_BACK_LIGHT]);
    m_brushes[BRUSH_DARK].CreateSolidBrush(m_colors[COLOR_BACK_DARK]);
    m_brushes[BRUSH_SELECTED].CreateSolidBrush(m_colors[COLOR_BACK_SELECTED]);
    m_brushes[BRUSH_HOVERED].CreateSolidBrush(m_colors[COLOR_BACK_HOVERED]);
}

void CDrawManager::CreatePens()
{
    LOG_DEBUG("creating pens");

    m_pens[PEN_NORMAL].CreatePen(PS_SOLID, 1, m_colors[COLOR_RECT_NORMAL]);
    m_pens[PEN_SELECTED].CreatePen(PS_SOLID, 1, m_colors[COLOR_RECT_SELECTED]);
    m_pens[PEN_HOVERED].CreatePen(PS_SOLID, 1, m_colors[COLOR_RECT_HOVERED]);
    m_pens[PEN_DISABLED].CreatePen(PS_SOLID, 1, m_colors[COLOR_RECT_DISABLED]);
    m_pens[PEN_STATIC].CreatePen(PS_SOLID, 1, m_colors[COLOR_RECT_STATIC]);
}

void CDrawManager::CreateFonts()
{
    LOG_DEBUG("creating fonts");

    WTL::CLogFont logFontMain;
    WTL::CLogFont logFontMenu;
    WTL::CLogFont logFontSmall;

    CreateLogFont(logFontMain,
                  cfg::skin::font::main::use_menu_font,
                  cfg::skin::font::main::facename,
                  cfg::skin::font::main::height);

    CreateLogFont(logFontMenu,
                  cfg::skin::font::menu::use_menu_font,
                  cfg::skin::font::menu::facename,
                  cfg::skin::font::menu::height);

    CreateLogFont(logFontSmall,
                  cfg::skin::font::small::use_menu_font,
                  cfg::skin::font::small::facename,
                  cfg::skin::font::small::height);

    m_fonts[FONT_MAIN].CreateFontIndirect(&logFontMain);
    m_fonts[FONT_MENU].CreateFontIndirect(&logFontMenu);
    m_fonts[FONT_SMALL].CreateFontIndirect(&logFontSmall);

    logFontMain.SetBold();
    logFontMenu.SetBold();
    logFontSmall.SetBold();

    m_fonts[FONT_MAIN_BOLD].CreateFontIndirect(&logFontMain);
    m_fonts[FONT_MENU_BOLD].CreateFontIndirect(&logFontMenu);
    m_fonts[FONT_SMALL_BOLD].CreateFontIndirect(&logFontSmall);
}

void CDrawManager::DestroyBrushes()
{
    LOG_DEBUG("destroying brushes");

    for(std::size_t i = 0; i < m_brushes.size(); ++i)
    {
        if(!m_brushes[i].IsNull())
            m_brushes[i].DeleteObject();
    }
}

void CDrawManager::DestroyPens()
{
    LOG_DEBUG("destroying pens");

    for(std::size_t i = 0; i < m_pens.size(); ++i)
    {
        if(!m_pens[i].IsNull())
            m_pens[i].DeleteObject();
    }
}

void CDrawManager::DestroyFonts()
{
    LOG_DEBUG("destroying fonts");

    for(std::size_t i = 0; i < m_fonts.size(); ++i)
    {
        if(!m_fonts[i].IsNull())
            m_fonts[i].DeleteObject();
    }
}

void CDrawManager::CreateLogFont(WTL::CLogFont& logFont, bool useMenuFont,
                                 std::wstring const& faceName, LONG height)
{
    if(useMenuFont)
    {
        logFont.SetMenuFont();
    }
    else
    {
        WTL::CClientDC desktopDC(HWND_DESKTOP);

        logFont.lfWeight = FW_NORMAL;
        logFont.lfCharSet = RUSSIAN_CHARSET;

        logFont.SetHeight(height, desktopDC);
        ::lstrcpy(logFont.lfFaceName, faceName.c_str());
    }
}
