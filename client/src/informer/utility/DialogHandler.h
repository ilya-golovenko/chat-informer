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
    typename Invoker
>
class DialogHandler_0
{
public:
    DialogHandler_0(Function function) :
        m_function(function)
    {
    }

    ~DialogHandler_0()
    {
    }

    DialogHandler_0(DialogHandler_0&& other) :
        m_function(std::move(other.m_function))
    {
    }

    DialogHandler_0(DialogHandler_0 const& other) :
        m_function(other.m_function)
    {
    }

    void operator()()
    {
        Invoker::Invoke(m_function);
    }

private:
    Function m_function;
};

template
<
    typename Dialog,
    typename Function,
    typename Invoker,
    typename Arg0
>
class DialogHandler_1
{
public:
    DialogHandler_1(Function function,
                    Arg0 const& arg0) :
        m_function(function),
        m_arg0(arg0)
    {
    }

    ~DialogHandler_1()
    {
    }

    DialogHandler_1(DialogHandler_1&& other) :
        m_function(std::move(other.m_function)),
        m_arg0(std::move(other.m_arg0))
    {
    }

    DialogHandler_1(DialogHandler_1 const& other) :
        m_function(other.m_function),
        m_arg0(other.m_arg0)
    {
    }

    void operator()()
    {
        Invoker::Invoke(m_function, m_arg0);
    }

private:
    Function m_function;
    Arg0 const m_arg0;
};

template
<
    typename Dialog,
    typename Function,
    typename Invoker,
    typename Arg0,
    typename Arg1
>
class DialogHandler_2
{
public:
    DialogHandler_2(Function function,
                    Arg0 const& arg0,
                    Arg1 const& arg1) :
        m_function(function),
        m_arg0(arg0),
        m_arg1(arg1)
    {
    }

    ~DialogHandler_2()
    {
    }

    DialogHandler_2(DialogHandler_2&& other) :
        m_function(std::move(other.m_function)),
        m_arg0(std::move(other.m_arg0)),
        m_arg1(std::move(other.m_arg1))
    {
    }

    DialogHandler_2(DialogHandler_2 const& other) :
        m_function(other.m_function),
        m_arg0(other.m_arg0),
        m_arg1(other.m_arg1)
    {
    }

    void operator()()
    {
        Invoker::Invoke(m_function, m_arg0, m_arg1);
    }

private:
    Function m_function;
    Arg0 const m_arg0;
    Arg1 const m_arg1;
};
