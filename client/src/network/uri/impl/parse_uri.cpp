//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/uri/impl/parse_uri.hpp>
#include <network/uri/impl/uri_grammar.hpp>

// STL headers
#include <stdexcept>


namespace chat
{
namespace net
{
namespace impl
{

void parse_uri(uri_parts& parts, std::string const& str)
{
    static uri_grammar<std::string::const_iterator> const grammar;

    if(!boost::spirit::qi::parse(str.begin(), str.end(), grammar, parts))
        throw std::invalid_argument("invalid uri string: " + str);
}

}   // namespace impl
}   // namespace net
}   // namespace chat
