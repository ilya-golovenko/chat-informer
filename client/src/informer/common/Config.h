//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <config/option.hpp>

// Windows headers
#include <windows.h>

// STL headers
#include <vector>
#include <string>


namespace cfg
{
namespace logging
{

static config::option<std::string> format("logging.format");
static config::option<std::string> filename("logging.filename");
static config::option<std::string> severity("logging.severity");
static config::option<std::size_t> max_size("logging.max_size");
static config::option<std::size_t> max_index("logging.max_index");

}   // namespace logging

namespace chat
{

static config::option<std::wstring> nickname("chat.nickname");
static config::option<std::wstring> password("chat.password");
static config::option<bool> bad_credentials("chat.bad_credentials");
static config::option<std::vector<std::wstring> > alarms("chat.alarms");

}   // namespace chat

namespace windows
{
namespace main
{

static config::option<int> top("windows.main.top", 0);
static config::option<int> left("windows.main.left", INT_MAX);
static config::option<int> width("windows.main.width", 180);
static config::option<int> height("windows.main.height", INT_MAX);

}   // namespace main

namespace forum
{

static config::option<int> top("windows.forum_top", INT_MAX);
static config::option<int> left("windows.forum.left", INT_MAX);
static config::option<int> width("windows.forum.width", 600);
static config::option<int> height("windows.forum.height", 200);

}   // namespace forum

namespace photoalbum
{

static config::option<int> width("windows.photoalbum.width", 660);
static config::option<int> height("windows.photoalbum.height", 480);

}   // namespace photoalbum

namespace notification
{

static config::option<int> top("windows.notification.top", INT_MAX);
static config::option<int> left("windows.notification.left", INT_MAX);

}   // namespace notification
}   // namespace windows

namespace hotkeys
{
namespace main
{

static config::option<UINT> modifiers("hotkeys.main.modifiers", MOD_CONTROL);
static config::option<UINT> virtual_key("hotkeys.main.virtual_key", VK_F1);

}   // namespace main

namespace forum
{

static config::option<UINT> modifiers("hotkeys.forum.modifiers", MOD_CONTROL);
static config::option<UINT> virtual_key("hotkeys.forum.virtual_key", VK_F2);

}   // namespace forum

namespace news
{

static config::option<UINT> modifiers("hotkeys.news.modifiers", MOD_CONTROL);
static config::option<UINT> virtual_key("hotkeys.news.virtual_key", VK_F3);

}   // namespace news

namespace photos
{

static config::option<UINT> modifiers("hotkeys.photos.modifiers", MOD_CONTROL);
static config::option<UINT> virtual_key("hotkeys.photos.virtual_key", VK_F4);

}   // namespace photos
}   // namespace hotkeys

namespace connection
{

static config::option<bool> use_proxy("connection.use_proxy", false);

namespace proxy
{

static config::option<bool> use_system("connection.proxy.use_system", true);

static config::option<int> port("connection.proxy.port", 3128);
static config::option<std::string> server("connection.proxy.server");

static config::option<bool> authenticate("connection.proxy.authenticate", false);
static config::option<std::string> username("connection.proxy.username");
static config::option<std::string> password("connection.proxy.password");

}   // namespace proxy
}   // namespace connection

namespace forum
{

static config::option<std::size_t> max_topics("forum.max_topics", 200);

}   // namespace forum

namespace common
{

static config::option<bool> autorun("common.autorun", true);
static config::option<bool> topmost("common.topmost", true);

static config::option<std::wstring> skin("common.skin", L"default");

static config::option<bool> sort_by_nicknames("common.sort_by_nicknames", false);
static config::option<bool> download_thumbnails("common.download_thumbnails", true);

}   // namespace common

namespace skin
{
namespace color
{
namespace back
{

static config::option<COLORREF> white("skin.colors.back.white", RGB(255, 255, 255));
static config::option<COLORREF> light("skin.colors.back.light", RGB(255, 239, 223));
static config::option<COLORREF> dark("skin.colors.back.dark", RGB(244, 218, 202));
static config::option<COLORREF> selected("skin.colors.back.selected", RGB(255, 213, 170));
static config::option<COLORREF> hovered("skin.colors.back.hovered", RGB(255, 228, 202));

}   // namespace back

namespace rect
{

static config::option<COLORREF> normal("skin.colors.rect.normal", RGB(128, 19, 0));
static config::option<COLORREF> selected("skin.colors.rect.selected", RGB(255, 149, 43));
static config::option<COLORREF> hovered("skin.colors.rect.hovered", RGB(213, 69, 0));
static config::option<COLORREF> disabled("skin.colors.rect.disabled", RGB(255, 255, 255));
static config::option<COLORREF> static_("skin.colors.rect.static", RGB(215, 190, 175));

}   // namespace rect

namespace text
{

static config::option<COLORREF> normal("skin.colors.text.normal", RGB(106, 0, 0));
static config::option<COLORREF> selected("skin.colors.text.selected", RGB(106, 0, 0));
static config::option<COLORREF> hovered("skin.colors.text.hovered", RGB(106, 0, 0));
static config::option<COLORREF> disabled("skin.colors.text.disabled", RGB(150, 150, 150));

}   // namespace text
}   // namespace color

namespace font
{
namespace main
{

static config::option<bool> use_menu_font("skin.fonts.main.use_menu_font", false);
static config::option<std::wstring> facename("skin.fonts.main.facename", L"Tahoma");
static config::option<LONG> height("skin.fonts.main.height", 10);

}   // namespace main

namespace menu
{

static config::option<bool> use_menu_font("skin.fonts.menu.use_menu_font", true);
static config::option<std::wstring> facename("skin.fonts.menu.facename", L"Tahoma");
static config::option<LONG> height("skin.fonts.menu.height", 10);

}   // namespace menu

namespace small
{

static config::option<bool> use_menu_font("skin.fonts.small.use_menu_font", false);
static config::option<std::wstring> facename("skin.fonts.small.facename", L"Tahoma");
static config::option<LONG> height("skin.fonts.small.height", 8);

}   // namespace small
}   // namespace font
}   // namespace skin
}   // namespace cfg
