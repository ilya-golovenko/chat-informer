//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/managers/ManagerBase.h>

// BOOST headers
#include <boost/array.hpp>


// Colors
enum
{
    COLOR_BACK_WHITE,
    COLOR_BACK_LIGHT,
    COLOR_BACK_DARK,
    COLOR_BACK_SELECTED,
    COLOR_BACK_HOVERED,

    COLOR_RECT_NORMAL,
    COLOR_RECT_SELECTED,
    COLOR_RECT_HOVERED,
    COLOR_RECT_DISABLED,
    COLOR_RECT_STATIC,

    COLOR_TEXT_NORMAL,
    COLOR_TEXT_SELECTED,
    COLOR_TEXT_HOVERED,
    COLOR_TEXT_DISABLED,

    COLOR_COUNT
};

// Brushes
enum
{
    BRUSH_INVALID = -1,

    BRUSH_WHITE,
    BRUSH_LIGHT,
    BRUSH_DARK,
    BRUSH_SELECTED,
    BRUSH_HOVERED,

    BRUSH_COUNT
};

// Pens
enum
{
    PEN_INVALID = -1,

    PEN_NORMAL,
    PEN_SELECTED,
    PEN_HOVERED,
    PEN_DISABLED,
    PEN_STATIC,

    PEN_COUNT
};

// Fonts
enum
{
    FONT_MAIN,
    FONT_MAIN_BOLD,

    FONT_MENU,
    FONT_MENU_BOLD,

    FONT_SMALL,
    FONT_SMALL_BOLD,

    FONT_COUNT
};

class CDrawManager :
    public CManagerBase<CDrawManager>
{
public:
    CDrawManager();

    virtual void Initialize();
    virtual void Finalize();

    COLORREF Color(std::size_t color);
    HBRUSH Brush(std::size_t brush);
    HPEN Pen(std::size_t pen);
    HFONT Font(std::size_t font);

private:
    void CreateColors();
    void CreateBrushes();
    void CreatePens();
    void CreateFonts();

    void DestroyBrushes();
    void DestroyPens();
    void DestroyFonts();

    void CreateLogFont(WTL::CLogFont& logFont, bool useMenuFont,
                       std::wstring const& faceName, LONG height);

private:
    boost::array<COLORREF, COLOR_COUNT> m_colors;
    boost::array<WTL::CBrush, BRUSH_COUNT> m_brushes;
    boost::array<WTL::CPen, PEN_COUNT> m_pens;
    boost::array<WTL::CFont, FONT_COUNT> m_fonts;
};
