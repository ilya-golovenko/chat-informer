//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//--------------------------------------------------------------------------- 
#ifndef _chat_network_http_constants_hpp
#define _chat_network_http_constants_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/asio.hpp>

// STL headers
#include <functional>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

namespace common
{

std::string const empty                     = "";
std::string const slash                     = "/";
std::string const space                     = " ";
std::string const separator                 = ": ";
std::string const crlf                      = "\r\n"; 

}   // namespace common

namespace request_methods
{

std::string const head                      = "HEAD";
std::string const get                       = "GET";
std::string const post                      = "POST";
std::string const put                       = "PUT";
std::string const delete_                   = "DELETE";

}   // namespace request_methods

namespace header_names
{

// common headers
std::string const date                      = "Date";
std::string const pragma                    = "Pragma";
std::string const upgrade                   = "Upgrade";
std::string const connection                = "Connection";
std::string const content_type              = "Content-Type";
std::string const cache_control             = "Cache-Control";
std::string const content_length            = "Content-Length";
std::string const proxy_connection          = "Proxy-Connection";
std::string const transfer_encoding         = "Transfer-Encoding";

// request-only headers
std::string const host                      = "Host";
std::string const range                     = "Range";
std::string const accept                    = "Accept";
std::string const referer                   = "Referer";
std::string const user_agent                = "User-Agent";
std::string const set_cookie                = "Set-Cookie";
std::string const authorization             = "Authorization";
std::string const proxy_authorization       = "Proxy-Authorization";

// response-only headers
std::string const server                    = "Server";
std::string const cookie                    = "Cookie";
std::string const location                  = "Location";
std::string const content_range             = "Content-Range";
std::string const www_authenticate          = "WWW-Authenticate";
std::string const proxy_authenticate        = "Proxy-Authenticate";

}   // namespace header_names

namespace cache_tokens
{

std::string const no_cache                  = "no-cache";
std::string const no_store                  = "no-store";

}   // namespace cache_tokens

namespace connection_tokens
{

std::string const close                     = "close";
std::string const upgrade                   = "upgrade";
std::string const keep_alive                = "keep-alive";

}   // namespace connection_tokens

namespace encoding_tokens
{

std::string const chunked                   = "chunked";
std::string const identity                  = "identity";
std::string const compress                  = "compress";
std::string const deflate                   = "deflate";
std::string const gzip                      = "gzip";

}   // namespace encoding_tokens

std::uint16_t const http_port_number        = 80;
std::uint16_t const https_port_number       = 443;

std::string const http_scheme_name          = "http";
std::string const https_scheme_name         = "https";

std::size_t const max_redirect_count        = 5;

std::size_t const max_message_size          = 32768;
std::size_t const max_header_name_length    = 1024;
std::size_t const max_header_value_length   = 8192;
std::size_t const max_chunk_size_length     = 1024;

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_constants_hpp
