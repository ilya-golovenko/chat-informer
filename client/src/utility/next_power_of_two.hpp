//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_next_power_of_two_hpp
#define _chat_utility_next_power_of_two_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <cstddef>


namespace chat
{

template <typename T>
T next_power_of_two(T value)
{
    if(value)
    {
        --value;

        for(std::size_t i = 1; i < sizeof(T) * 8; i <<= 8)
        {
            value |= value >> i;
            value |= value >> (i << 1);
            value |= value >> (i << 2);
            value |= value >> (i << 3);
            value |= value >> (i << 4);
            value |= value >> (i << 5);
            value |= value >> (i << 6);
            value |= value >> (i << 7);
        }
    }

    return ++value;
}

}   // namespace chat

#endif  // _chat_utility_next_power_of_two_hpp
