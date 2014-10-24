//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_util_bind_handler_hpp
#define _chat_network_util_bind_handler_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/bind_traits.hpp>

// STL headers
#include <memory>


namespace chat
{
namespace net
{

template <typename Object, typename Function>
auto bind_handler(std::shared_ptr<Object> object, Function function)
{
    return typename impl::bind_traits<Object, Function>::binder_type(object, function);
}

template <typename Object, typename Function>
auto bind_weak_handler(std::shared_ptr<Object> object, Function function)
{
    return typename impl::weak_bind_traits<Object, Function>::binder_type(object, function);
}

}   // namespace net
}   // namespace chat

#endif  // _chat_network_util_bind_handler_hpp
