//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/impl/headers_parser.hpp>


namespace chat
{
namespace net
{
namespace http
{
namespace impl
{

headers_parser::headers_parser() :
    state_(state_header_line_start)
{
}

void headers_parser::reset()
{
    state_= state_header_line_start;
    name_.clear();
    value_.clear();
}

parse_result headers_parser::consume(message& message, char c)
{
    switch(state_)
    {
        case state_header_line_start:
            if(c == '\r')
            {
                state_ = state_expecting_newline_2;
                return parse_result::more;
            }
            else if(c == ' ' || c == '\t')
            {
                if(!name_.empty())
                {
                    state_ = state_header_whitespace;
                    return parse_result::more;
                }
            }
            else if(is_char(c))
            {
                name_.assign(1, c);
                state_ = state_header_name;
                return parse_result::more;
            }
            break;

        case state_header_whitespace:
            if(c == '\r')
            {
                state_ = state_expecting_newline_1;
                return parse_result::more;
            }
            else if(c == ' ' || c == '\t')
            {
                return parse_result::more;
            }
            else if(!is_control(c))
            {
                if(value_.size() < max_header_value_length)
                {
                    value_.push_back(c);
                    state_ = state_header_value;
                    return parse_result::more;
                }
            }
            break;

        case state_header_name:
            if(c == ':')
            {
                state_ = state_space_before_header_value;
                return parse_result::more;
            }
            else if(is_char(c))
            {
                if(name_.size() < max_header_name_length)
                {
                    name_.push_back(c);
                    return parse_result::more;
                }
            }
            break;

        case state_space_before_header_value:
            if(c == ' ')
            {
                state_ = state_header_value_start;
                return parse_result::more;
            }
            break;

        case state_header_value_start:
            if(!is_control(c))
            {
                value_.assign(1, c);
                state_ = state_header_value;
                return parse_result::more;
            }
            break;

        case state_header_value:
            if(c == '\r')
            {
                message.add(name_, value_);
                state_ = state_expecting_newline_1;
                return parse_result::more;
            }
            else if(!is_control(c))
            {
                if(value_.size() < max_header_value_length)
                {
                    value_.push_back(c);
                    return parse_result::more;
                }
            }
            break;

        case state_expecting_newline_1:
            if(c == '\n')
            {
                state_ = state_header_line_start;
                return parse_result::more;
            }
            break;

        case state_expecting_newline_2:
            if(c == '\n')
            {
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
