//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/impl/request_parser.hpp>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

request_parser::request_parser() :
    state_(state_method_start),
    size_(0u)
{
}

void request_parser::reset()
{
    state_ = state_method_start;
    size_ = 0u;
    method_.clear();
    url_.clear();
    version_parser_.reset();
    headers_parser_.reset();
}

parse_result request_parser::consume(request& request, char c)
{
    if(++size_ > max_message_size)
        return parse_result::failure;

    switch(state_)
    {
        case state_method_start:
            if(is_char(c))
            {
                method_.push_back(c);
                state_ = state_method;
                return parse_result::more;
            }
            break;

        case state_method:
            if(c == ' ')
            {
                request.set_method(method_);
                state_ = state_url_start;
                return parse_result::more;
            }
            else if(is_char(c))
            {
                method_.push_back(c);
                return parse_result::more;
            }
            break;

        case state_url_start:
            if(!is_control(c))
            {
                url_.push_back(c);
                state_ = state_url;
                return parse_result::more;
            }
            break;

        case state_url:
            if(c == ' ')
            {
                request.set_url(url_);
                state_ = state_http_version;
                return parse_result::more;
            }
            else if(!is_control(c))
            {
                url_.push_back(c);
                return parse_result::more;
            }
            break;

        case state_http_version:
            switch(version_parser_.consume(request, c))
            {
                case parse_result::ok:
                    if(c == '\r')
                    {
                        state_ = state_expecting_newline;
                        return parse_result::more;
                    }
                    break;

                case parse_result::more:
                    return parse_result::more;

                default:
                    ;
            }
            break;

        case state_expecting_newline:
            if(c == '\n')
            {
                state_ = state_headers;
                return parse_result::more;
            }
            break;

        case state_headers:
            return headers_parser_.consume(request, c);
    }

    return parse_result::failure;
}

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat
