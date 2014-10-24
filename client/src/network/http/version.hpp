//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_version_hpp
#define _chat_network_http_version_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <iostream>
#include <cstdint>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class version
{
public:
    version();

    version(std::uint8_t major, std::uint8_t minor);

    version(version const&) = default;
    version& operator=(version const&) = default;

    std::uint8_t get_major() const;
    std::uint8_t get_minor() const;

    void set_major(std::uint8_t major);
    void set_minor(std::uint8_t minor); 

    std::string to_string() const;

    friend bool operator==(version const& lhs, version const& rhs);
    friend bool operator<(version const& lhs, version const& rhs);

private:
    std::uint8_t major_;
    std::uint8_t minor_;
};

inline bool operator!=(version const& lhs, version const& rhs) { return !operator==(lhs, rhs); }
inline bool operator<=(version const& lhs, version const& rhs) { return !operator<(rhs, lhs); }
inline bool operator>=(version const& lhs, version const& rhs) { return !operator<(lhs, rhs); }
inline bool operator>(version const& lhs, version const& rhs) { return operator<(rhs, lhs); }

inline std::ostream& operator<<(std::ostream& os, version const& version)
{
    os << version.to_string();
    return os;
}

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_version_hpp
