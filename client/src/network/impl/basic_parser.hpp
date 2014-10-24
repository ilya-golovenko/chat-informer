//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_impl_basic_parser_hpp
#define _chat_network_impl_basic_parser_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/parser_base.hpp>

// STL headers
#include <utility>


namespace chat
{
namespace net
{
namespace impl
{

template <typename Parser>
class basic_parser : public parser_base
{
public:
    basic_parser() = default;
    ~basic_parser() = default;

    basic_parser(basic_parser const&) = delete;
    basic_parser& operator=(basic_parser const&) = delete;

    template <typename Iterator>
    std::pair<parse_result, Iterator> parse(Iterator begin, Iterator end);

    template <typename Message, typename Iterator>
    std::pair<parse_result, Iterator> parse(Message& message, Iterator begin, Iterator end);
};

template <typename Parser>
template <typename Iterator>
std::pair<parse_result, Iterator> basic_parser<Parser>::parse(Iterator begin, Iterator end)
{
    parse_result result = parse_result::more;
    Parser& impl = static_cast<Parser&>(*this);

    while(begin != end)
    {
        result = impl.consume(*begin++);

        if(result != parse_result::more)
        {
            impl.reset();
            break;
        }
    }

    return { result, begin };
}

template <typename Parser>
template <typename Message, typename Iterator>
std::pair<parse_result, Iterator> basic_parser<Parser>::parse(Message& message, Iterator begin, Iterator end)
{
    parse_result result = parse_result::more;
    Parser& impl = static_cast<Parser&>(*this);

    while(begin != end)
    {
        result = impl.consume(message, *begin++);

        if(result != parse_result::more)
        {
            impl.reset();
            break;
        }
    }

    return { result, begin };
}

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_impl_basic_parser_hpp
