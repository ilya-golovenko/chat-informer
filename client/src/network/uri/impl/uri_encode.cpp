//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/uri/impl/uri_encode.hpp>

// STL headers
#include <cstring>


namespace net
{
namespace uri
{
namespace impl
{

static char const encode_table[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char const hex_chars[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static int hex_to_int(int c)
{
    if(c >= '0' && c <= '9')
        return c - '0';

    if(c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return -1;
}

static std::string encode(std::string const& str, char const* ignore)
{
    std::string result;

    for(unsigned char const c : str)
    {
        if(encode_table[c] && !std::strchr(ignore, c))
        {
            result.push_back('%');
            result.push_back(hex_chars[c >> 4]);
            result.push_back(hex_chars[c & 0xF]);
        }
        else
        {
            result.push_back(c);
        }
    }

    return result;
}

std::string encode_user_info(std::string const& user_info)
{
    return encode(user_info, ":");
}

std::string encode_host(std::string const& host)
{
    return encode(host, "[:]");
}

std::string encode_path(std::string const& path)
{
    return encode(path, "/.@%;=");
}

std::string encode_query(std::string const& query)
{
    return encode(query, "/.@&%;=");
}

std::string encode_fragment(std::string const& fragment)
{
    return encode(fragment, "/.@&l;=%");
}

std::string decode(std::string const& str)
{
    std::string result;

    for(std::size_t i = 0; i < str.size(); ++i)
    {
        switch(str[i])
        {
            case '%':
                if(i + 2 < str.size())
                {
                    int const h = hex_to_int(str[++i]);
                    int const l = hex_to_int(str[++i]);

                    if(h >= 0 && l >= 0)
                    {
                        int const v = (h << 4) | l;

                        if(v >= 32 && v != 127)
                            result.push_back(char(v));
                    }
                }
                else
                {
                    // ignore invalid sequence
                    result.push_back('%');
                }
                break;

            case '+':
                result.push_back(' ');
                break;

            default:
                result.push_back(str[i]);
        }
    }

    return result;
}

}   // namespace impl
}   // namespace net
}   // namespace chat
