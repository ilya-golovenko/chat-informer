//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <utility/html_utils.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>


namespace chat
{

//TODO: reuse boost spirit qi & karam here

std::string escape_html(std::string const& string)
{
    std::string output(string);

    boost::algorithm::replace_all(output, "<", "&lt;");
    boost::algorithm::replace_all(output, ">", "&gt;");
    boost::algorithm::replace_all(output, "&", "&amp;");
    boost::algorithm::replace_all(output, "\"", "&quot;");

    return output;
}

std::string unescape_html(std::string const& string)
{
    std::string output(string);

    boost::algorithm::replace_all(output, "&lt;", "<");
    boost::algorithm::replace_all(output, "&gt;", ">");
    boost::algorithm::replace_all(output, "&amp;", "&");
    boost::algorithm::replace_all(output, "&nbsp;", " ");
    boost::algorithm::replace_all(output, "&quot;", "\"");

    return output;
}

}   // namespace chat
