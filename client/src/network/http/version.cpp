//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/version.hpp>

// MISSIO headers
#include <missio/format/write.hpp>


namespace chat
{
namespace net
{
namespace http
{

version::version() :
    major_(1u),
    minor_(1u)
{
}

version::version(std::uint8_t major, std::uint8_t minor) :
    major_(major),
    minor_(minor)
{
}

std::uint8_t version::get_major() const
{
    return major_;
}

std::uint8_t version::get_minor() const
{
    return minor_;
}

void version::set_major(std::uint8_t major)
{
    major_ = major;
}

void version::set_minor(std::uint8_t minor)
{
    minor_ = minor;
}

std::string version::to_string() const
{
    std::string string;

    missio::format::write(string, "HTTP/", major_, ".", minor_);

    return string;
}

bool operator==(version const& lhs, version const& rhs)
{
    return lhs.major_ == rhs.major_ && lhs.minor_ == rhs.minor_;
}

bool operator<(version const& lhs, version const& rhs)
{
    return lhs.major_ == rhs.major_ ? lhs.minor_ < rhs.minor_ : lhs.major_ < rhs.major_;
}

}   // namespace http
}   // namespace net
}   // namespace chat
