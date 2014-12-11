//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// STL headers
#include <utility>


template
<
    typename Dialog,
    typename Function,
    typename Invoker,
    typename ...Args
>
class DialogHandler
{
public:
    DialogHandler(Function function, Args const& ...args) :
        m_function(function),
        m_args(args...)
    {
    }

    DialogHandler(DialogHandler&& other) :
        m_function(std::move(other.m_function)),
        m_args(std::move(other.m_args)...)
    {
    }

    DialogHandler(DialogHandler const& other) :
        m_function(other.m_function),
        m_args(other.m_args...)
    {
    }

    void operator()()
    {
        Invoker::Invoke(m_function, m_args...);
    }

private:
    Function m_function;
    Args m_args...;
};
