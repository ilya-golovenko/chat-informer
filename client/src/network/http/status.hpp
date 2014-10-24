//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_http_status_hpp
#define _chat_network_http_status_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// BOOST headers
#include <boost/optional.hpp>

// STL headers
#include <iostream>
#include <string>


namespace chat
{
namespace net
{
namespace http
{

class status
{
public:
    enum code_type
    {
        continue_                       = 100,
        switching_protocols             = 101,

        ok                              = 200,
        created                         = 201,
        accepted                        = 202,
        non_authoritative               = 203,
        no_content                      = 204,
        reset_content                   = 205,
        partial_content                 = 206,

        multiple_choices                = 300,
        moved                           = 301,
        found                           = 302,
        see_other                       = 303,
        not_modified                    = 304,
        use_proxy                       = 305,
        temporary_redirect              = 307,

        bad_request                     = 400,
        unauthorized                    = 401,
        forbidden                       = 403,
        not_found                       = 404,
        method_not_allowed              = 405,
        not_acceptable                  = 406,
        proxy_authentication_required   = 407,
        request_timeout                 = 408,
        conflict                        = 409,
        gone                            = 410,
        length_required                 = 411,
        precondition_failed             = 412,
        request_entity_too_large        = 413,
        request_uri_too_long            = 414,
        unsupported_media_type          = 415,
        requested_range_not_satisfiable = 416,
        expectation_failed              = 417,

        internal_server_error           = 500,
        not_implemented                 = 501,
        bad_gateway                     = 502,
        service_unavailable             = 503,
        gateway_timeout                 = 504,
        version_not_supported           = 505
    };

public:
    status();

    status(std::uint16_t code);
    status(std::uint16_t code, std::string const& reason);

    status(status const&) = default;
    status& operator=(status const&) = default;

    status(status&&) = default;
    status& operator=(status&&) = default;

    std::uint16_t get_code() const;
    void set_code(std::uint16_t code);

    std::string const& get_reason() const;
    void set_reason(std::string const& reason);

    bool is_provisional() const;
    bool is_successful() const;
    bool is_redirected() const;
    bool is_request_error() const;
    bool is_server_error() const;

    std::string to_string() const;

    friend bool operator==(status const& lhs, status const& rhs);
    friend bool operator<(status const& lhs, status const& rhs);

private:
    std::uint16_t code_;
    boost::optional<std::string> reason_;
};

inline bool operator!=(status const& lhs, status const& rhs) { return !(lhs == rhs); }
inline bool operator<=(status const& lhs, status const& rhs) { return !(rhs < lhs); }
inline bool operator>=(status const& lhs, status const& rhs) { return !(lhs < rhs); }
inline bool operator>(status const& lhs, status const& rhs) { return (rhs < lhs); }

inline std::ostream& operator<<(std::ostream& os, status const& status)
{
    os << status.to_string();
    return os;
}

}   // namespace http
}   // namespace net
}   // namespace chat

#endif  // _chat_network_http_status_hpp
