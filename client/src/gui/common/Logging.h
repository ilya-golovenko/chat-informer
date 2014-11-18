//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2012, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <missio/logging/common.hpp>

// ATL/WTL headers
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>


namespace missio
{
namespace format
{
namespace detail
{

template <>
struct type_adapter<WTL::CString>
{
    template <typename Sink>
    static void format(Sink& sink, WTL::CString const& value)
    {
        write(sink, static_cast<LPCWSTR>(value));
    }
};

}   // namespace detail
}   // namespace format
}   // namespace missio
