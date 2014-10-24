//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/impl/version_parser.hpp>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

version_parser::version_parser() :
    state_(state_version_h),
    major_(0u),
    minor_(0u)
{
}

void version_parser::reset()
{
    state_= state_version_h;
    major_ = 0u;
    minor_ = 0u;
}

parse_result version_parser::consume(message& message, char c)
{
    switch(state_)
    {
        case state_version_h:
            if(c == 'H')
            {
                state_ = state_version_t_1;
                return parse_result::more;
            }
            break;

        case state_version_t_1:
            if(c == 'T')
            {
                state_ = state_version_t_2;
                return parse_result::more;
            }
            break;

        case state_version_t_2:
            if(c == 'T')
            {
                state_ = state_version_p;
                return parse_result::more;
            }
            break;

        case state_version_p:
            if(c == 'P')
            {
                state_ = state_version_slash;
                return parse_result::more;
            }
            break;

        case state_version_slash:
            if(c == '/')
            {
                state_ = state_version_major_start;
                return parse_result::more;
            }
            break;

        case state_version_major_start:
            if(is_digit(c))
            {
                major_ = c - '0';
                state_ = state_version_major;
                return parse_result::more;
            }
            break;

        case state_version_major:
            if(c == '.')
            {
                state_ = state_version_minor_start;
                return parse_result::more;
            }
            else if(is_digit(c))
            {
                major_ = major_ * 10 + c - '0';
                return parse_result::more;
            }
            break;

        case state_version_minor_start:
            if(is_digit(c))
            {
                minor_ = c - '0';
                state_ = state_version_minor;
                return parse_result::more;
            }
            break;

        case state_version_minor:
            if(is_digit(c))
            {
                minor_ = minor_ * 10 + c - '0';
                return parse_result::more;
            }
            else
            {
                message.set_version(major_, minor_);
                return parse_result::ok;
            }
            break;
    }

    return parse_result::failure;
}

}   // namespace impl
}   // namespace http
}   // namespace net
}   // namespace chat
