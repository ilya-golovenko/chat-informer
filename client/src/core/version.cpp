//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/version.hpp>

// MISSIO headers
#include <missio/unicode/convert.hpp>
#include <missio/format/write.hpp>


namespace chat
{

version const version::unknown;

version::version(int major, int minor, int build) :
    major_(major),
    minor_(minor),
    build_(build)
{
}

int version::major() const
{
    return major_;
}

int version::minor() const
{
    return minor_;
}

int version::build() const
{
    return build_;
}

std::string version::to_string() const
{
    std::string result;

    missio::format::write(result, major_, '.', minor_, '.', build_);

    return result;
}

std::wstring version::to_wstring() const
{
    return missio::unicode::to_wide_string(to_string());
}

bool operator<(version const& lhs, version const& rhs)
{
    if(lhs.major() != rhs.major())
        return lhs.major() < rhs.major();

    if(lhs.minor() != rhs.minor())
        return lhs.minor() < rhs.minor();

    return lhs.build() < rhs.build();
}

bool operator==(version const& lhs, version const& rhs)
{
    return lhs.major() == rhs.major() &&
           lhs.minor() == rhs.minor() &&
           lhs.build() == rhs.build();
}

}   // namespace chat
