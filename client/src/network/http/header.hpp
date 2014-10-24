//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_header_hpp
#define _chat_network_http_header_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <iostream>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class header
{
public:
    header() = default;
    ~header() = default;

    header(std::string const& name, std::string const& value);

    header(header const&) = default;
    header& operator=(header const&) = default;

    header(header&&) = default;
    header& operator=(header&&) = default;

    std::string const& get_name() const;
    void set_name(std::string const& name);

    std::string const& get_value() const;
    void set_value(std::string const& value);

    void serialize(std::ostream& os) const;

    friend bool operator==(header const& lhs, header const& rhs);
    friend bool operator<(header const& lhs, header const& rhs); 

private:
    std::string name_;
    std::string value_;
};

inline bool operator!=(header const& lhs, header const& rhs) { return !(lhs == rhs); }
inline bool operator<=(header const& lhs, header const& rhs) { return !(rhs < lhs); }
inline bool operator>=(header const& lhs, header const& rhs) { return !(lhs < rhs); }
inline bool operator>(header const& lhs, header const& rhs) { return (rhs < lhs); }

inline std::ostream& operator<<(std::ostream& os, header const& header)
{
    header.serialize(os);
    return os;
}

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_header_hpp
