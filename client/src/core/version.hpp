//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _missio_core_version_hpp
#define _missio_core_version_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <iostream>
#include <string>


namespace missio
{

class version
{
public:
    version();
    ~version();

    version(int major,
            int minor,
            int build);

    version(version const& other);
    version& operator=(version const& other);

    int major() const;
    int minor() const;
    int build() const;

    std::string to_string() const;
    std::wstring to_wstring() const;

public:
    static version const unknown;

private:
    int major_;
    int minor_;
    int build_;
};

inline bool operator==(version const& lhs, version const& rhs)
{
    return (lhs.major() == rhs.major()
        && lhs.minor() == rhs.minor()
        && lhs.build() == rhs.build());
}

inline bool operator<(version const& lhs, version const& rhs)
{
    if(lhs.major() != rhs.major())
        return (lhs.major() < rhs.major());

    if(lhs.minor() != rhs.minor())
        return (lhs.minor() < rhs.minor());

    return (lhs.build() < rhs.build());
}

inline bool operator!=(version const& lhs, version const& rhs) { return !(lhs == rhs); }
inline bool operator<=(version const& lhs, version const& rhs) { return !(rhs < lhs); }
inline bool operator>=(version const& lhs, version const& rhs) { return !(lhs < rhs); }
inline bool operator>(version const& lhs, version const& rhs) { return (rhs < lhs); }

inline std::ostream& operator<<(std::ostream& os, version const& version)
{
    os << version.to_string();
    return os;
}

}   // namespace missio

#endif  // _missio_core_version_hpp
