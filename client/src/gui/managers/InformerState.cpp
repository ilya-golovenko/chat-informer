//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <informer/managers/InformerState.h>

// STL headers
#include <iostream>


std::ostream& operator<<(std::ostream& os, InformerState state)
{
    switch(state)
    {
        case InformerOnline:
            os << "online";
            break;

        case InformerStandby:
            os << "standby";
            break;

        case InformerOffline:
            os << "offline";
            break;

        default:
            os << "<unknown>";
    }

    return os;
}
