//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_impl_handler_binder_hpp
#define _chat_network_impl_handler_binder_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <utility>


namespace chat
{
namespace net
{
namespace impl
{

template
<
    typename Object,
    typename Function,
    typename Invoker
>
class handler_binder
{
public:
    handler_binder(Object object, Function function) :
        object_(object),
        function_(function)
    {
    }

    handler_binder(handler_binder const& other) = default;
    handler_binder& operator=(handler_binder const& other) = default;

    handler_binder(handler_binder&& other) = default;
    handler_binder& operator=(handler_binder&& other) = default;

    template <typename ...Args>
    void operator()(Args const& ...args) const
    {
        Invoker::call(object_, function_, args...);
    }

    template <typename ...Args>
    void operator()(Args const& ...args)
    {
        Invoker::call(object_, function_, args...);
    }

private:
    Object object_;
    Function function_;
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_impl_handler_binder_hpp
