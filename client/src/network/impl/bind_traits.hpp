//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_impl_bind_traits_hpp
#define _chat_network_impl_bind_traits_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <network/impl/handler_binder.hpp>
#include <network/impl/handler_invoker.hpp>

// STL headers
#include <memory>


namespace chat
{
namespace net
{
namespace impl
{

template <typename T, typename F>
struct bind_traits
{
    typedef handler_binder<std::shared_ptr<T>, F, handler_invoker<T, F>> binder_type;
};

template <typename T, typename F>
struct weak_bind_traits
{
    typedef handler_binder<std::weak_ptr<T>, F, weak_handler_invoker<T, F>> binder_type;
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_impl_bind_traits_hpp
