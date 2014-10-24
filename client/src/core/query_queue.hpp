//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_query_queue_hpp
#define _chat_core_query_queue_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// Application headers
#include <core/info.hpp>
#include <core/query.hpp>
#include <network/util/timer.hpp>

// MISSIO headers
#include <missio/logging/common.hpp>

// BOOST headers
#include <boost/asio.hpp>

// STL headers
#include <memory>
#include <vector>

/*
namespace chat
{

class query_queue: public std::enable_shared_from_this<query_queue>
{
public:
    typedef std::shared_ptr<query_queue> pointer;
    typedef std::vector<query::pointer> query_vector;

    typedef std::function<void (query_vector const&)> handler;

    static pointer create(boost::asio::io_service& io_service, handler const& handler);

public:
    query_queue(boost::asio::io_service& io_service, handler const& handler);

    ~query_queue();

    void add(query::pointer query);

    bool empty() const;
    void clear();

private:
    void handle_timer(boost::system::error_code const& error);

private:
    handler handler_;
    net::timer timer_;
    query_vector queries_;
};

}   // namespace chat
*/
#endif  // _chat_core_query_queue_hpp
