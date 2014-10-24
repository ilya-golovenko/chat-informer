//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_mime_types_hpp
#define _chat_network_mime_types_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <string>


namespace chat
{
namespace net
{
namespace mime
{

extern std::string const text_html;
extern std::string const text_plain;
extern std::string const text_xml;
extern std::string const text_css;

extern std::string const image_jpeg;
extern std::string const image_gif;
extern std::string const image_tiff;
extern std::string const image_png;

extern std::string const application_rtf;
extern std::string const application_pdf;
extern std::string const application_javascript;
extern std::string const application_octet_stream;
extern std::string const application_form_urlencoded;

std::string const& from_extension(std::string const& ext);

}   // namespace mime
}   // namespace net
}   // namespace chat

#endif  // _chat_network_mime_types_hpp
