//---------------------------------------------------------------------------
//
//    This file is part of missio project
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/md5/digest.hpp>
#include <utility/make_hex_string.hpp>


namespace crypto
{
namespace md5
{

digest::digest()
{
    std::memset(value_, 0, 16);
}

digest::digest(unsigned char const value[16])
{
    assign(value);
}

digest::digest(digest const& other)
{
    assign(other);
}

digest& digest::operator=(digest const& other)
{
    assign(other);
    return *this;
}

void digest::assign(digest const& other)
{
    if(&other != this)
        assign(other.value_);
}

void digest::assign(unsigned char const value[16])
{
    std::memcpy(value_, value, 16);
}

void digest::copy(unsigned char value[16]) const
{
    std::memcpy(value, value_, 16);
}

std::string digest::to_string() const
{
    return util::make_hex_string(value_, 16);
}

bool digest::operator<(digest const& other) const
{
    return (0 > std::memcmp(value_, other.value_, 16));
}

bool digest::operator==(digest const& other) const
{
    return (0 == std::memcmp(value_, other.value_, 16));
}

}   // namespace md5
}   // namespace crypto
