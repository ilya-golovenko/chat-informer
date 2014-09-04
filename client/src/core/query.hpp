//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_query_hpp
#define _chat_core_query_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/info.hpp>
#include <core/errors.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>

// STL headers
#include <ctime>


namespace chat
{

class query :
    private boost::noncopyable,
    public boost::enable_shared_from_this<query>
{
friend class informer;

public:
    typedef boost::shared_ptr<query> pointer;
    typedef boost::function<void (pointer)> handler;

public:
    static pointer create(missio::json::value const& data, bool need_auth, handler const& handler);

public:
    query(missio::json::value const& data, bool need_auth, handler const& handler);

    void cancel();
    bool is_completed();

    bool need_auth() const;
    error::type error() const;

    missio::json::value& json_data();
    missio::json::value const& json_data() const;
    void set_json_data(missio::json::value const& data);

private:
    void handle_info(error::type error);

private:
    std::time_t time_;
    error::type error_;
    missio::json::value data_;
    bool need_auth_;
    handler handler_;

private:
    volatile long completed_;
};

}   // namespace chat

#endif  // _chat_core_query_hpp
