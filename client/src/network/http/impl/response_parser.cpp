//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/impl/response_parser.hpp>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

response_parser::response_parser() :
    state_(state_version),
    size_(0u),
    status_(0u)
{
}

void response_parser::reset()
{
    state_ = state_version;
    size_ = 0u;
    status_ = 0u;
    version_parser_.reset();
    headers_parser_.reset();
}

parse_result response_parser::consume(response& response, char c)
{
    if(++size_ > max_message_size)
        return parse_result::failure;

    switch(state_)
    {
        case state_version:
            switch(version_parser_.consume(response, c))
            {
                case parse_result::ok:
                    if(c == ' ')
                    {
                        state_ = state_status_code_start;
                        return parse_result::more;
                    }
                    break;

                case parse_result::more:
                    return parse_result::more;

                default:
                    ;
            }
            break;

        case state_status_code_start:
            if(is_digit(c))
            {
                status_ = c - '0';
                state_ = state_status_code;
                return parse_result::more;
            }
            break;

        case state_status_code:
            if(c == ' ')
            {
                response.set_status(status_);
                state_ = state_status_reason;
                return parse_result::more;
            }
            else if(is_digit(c))
            {
                status_ = status_ * 10 + c - '0';
                return parse_result::more;
            }
            break;

        case state_status_reason:
            if(c == '\r')
            {
                state_ = state_expecting_newline;
                return parse_result::more;
            }
            else if(is_char(c))
            {
                return parse_result::more;
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
            return headers_parser_.consume(response, c);
    }

    return parse_result::failure;
}

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat
