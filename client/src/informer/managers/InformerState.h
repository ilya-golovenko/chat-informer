//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#pragma once

// STL headers
#include <iosfwd>


enum InformerState
{
    InformerOnline,
    InformerStandby,
    InformerOffline
};

std::ostream& operator<<(std::ostream& os, InformerState state);
