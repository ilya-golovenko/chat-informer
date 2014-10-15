//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// Application headers
#include <informer/utility/DialogBindTraits.h>


template <typename Function>
inline typename DialogBindTraits<Function>::BinderType BindDialogHandler(Function function)
{
    return typename DialogBindTraits<Function>::BinderType(function);
}
