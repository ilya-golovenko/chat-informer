//---------------------------------------------------------------------------
//
//    This file is part of missio project
//    Copyright (C) 2011 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _crypto_md5_digest_hpp
#define _crypto_md5_digest_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <iostream>
#include <string>


namespace crypto
{
namespace md5
{

class digest
{
public:
    digest();
    digest(unsigned char const value[16]);

    digest(digest const& other);
    digest& operator=(digest const& other);

    void assign(digest const& other);
    void assign(unsigned char const value[16]);

    void copy(unsigned char value[16]) const;

    std::string to_string() const;

    bool operator<(digest const& other) const;
    bool operator==(digest const& other) const;

private:
    unsigned char value_[16];
};

inline bool operator!=(digest const& lhs, digest const& rhs)
{
    return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& os, digest const& digest)
{
    os << digest.to_string();
    return os;
}

}   // namespace md5
}   // namespace crypto

#endif  // _crypto_md5_digest_hpp
