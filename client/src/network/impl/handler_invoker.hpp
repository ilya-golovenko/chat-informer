//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_impl_handler_invoker_hpp
#define _chat_network_impl_handler_invoker_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <memory>


namespace chat
{
namespace net
{
namespace impl
{

template <typename T, typename F>
struct handler_invoker
{
    template <typename ...Args>
    static void call(std::shared_ptr<T> const& object, F function, Args const& ...args)
    {
        ((*object).*function)(args...);
    }
};

template <typename T, typename F>
struct weak_handler_invoker
{
    template <typename ...Args>
    static void call(std::weak_ptr<T> const& weak_object, F function, Args const& ...args)
    {
        if(std::shared_ptr<T> object = weak_object.lock())
        {
            ((*object).*function)(args...);
        }
    }
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_impl_handler_invoker_hpp
