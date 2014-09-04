//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_version_hpp
#define _chat_core_version_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// STL headers
#include <iostream>
#include <string>


namespace chat
{

class version
{
public:
    version();
    ~version();

    version(int major, int minor, int build);

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

bool operator<(version const& lhs, version const& rhs);
bool operator==(version const& lhs, version const& rhs);

inline bool operator!=(version const& lhs, version const& rhs) { return !operator==(lhs, rhs); }
inline bool operator<=(version const& lhs, version const& rhs) { return !operator<(rhs, lhs); }
inline bool operator>=(version const& lhs, version const& rhs) { return !operator<(lhs, rhs); }
inline bool operator>(version const& lhs, version const& rhs) { return operator<(rhs, lhs); }

inline std::ostream& operator<<(std::ostream& os, version const& version)
{
    os << version.to_string();
    return os;
}

}   // namespace chat

#endif  // _chat_core_version_hpp
