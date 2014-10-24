//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/mime/types.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>


namespace chat
{
namespace net
{
namespace mime
{

std::string const text_html                     = "text/html";
std::string const text_plain                    = "text/plain";
std::string const text_xml                      = "text/xml";
std::string const text_css                      = "text/css";

std::string const image_jpeg                    = "image/jpeg";
std::string const image_gif                     = "image/gif";
std::string const image_tiff                    = "image/tiff";
std::string const image_png                     = "image/png";

std::string const application_rtf               = "application/rtf";
std::string const application_pdf               = "application/pdf";
std::string const application_javascript        = "application/javascript";
std::string const application_octet_stream      = "application/octet-stream";
std::string const application_form_urlencoded   = "application/x-www-form-urlencoded";

struct entry
{
    std::string const ext;
    std::string const& type;
};

entry const table[] =
{
    { ".html", text_html },
    { ".htm", text_html },
    { ".txt", text_plain },
    { ".xml", text_xml },
    { ".css", text_css },

    { ".jpeg", image_jpeg },
    { ".jpg", image_jpeg },
    { ".jpe", image_jpeg },
    { ".gif", image_gif },
    { ".tiff", image_tiff },
    { ".tif", image_tiff },
    { ".png", image_png },

    { ".rtf", application_rtf },
    { ".pdf", application_pdf },
    { ".js", application_javascript },
    { ".exe", application_octet_stream }
};

std::string const& from_extension(std::string const& ext)
{
    for(entry const& entry : table)
    {
        if(boost::algorithm::iequals(ext, entry.ext))
            return entry.type;
    }

    return application_octet_stream;
}

}   // namespace mime
}   // namespace net
}   // namespace chat
