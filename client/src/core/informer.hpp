//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_informer_hpp
#define _chat_core_informer_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// Application headers
#include <core/info.hpp>
#include <core/errors.hpp>
#include <core/credentials.hpp>
#include <core/version.hpp>
#include <core/query_queue.hpp>
#include <core/instance_id.hpp>
#include <crypto/manager.hpp>
#include <net/http/client_session.hpp>
#include <net/util/async_timer.hpp>
#include <net/util/thread_pool.hpp>

// MISSIO headers
#include <missio/json/json.hpp>

// BOOST headers
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

// STL headers
#include <string>


namespace chat
{

class informer :
    private boost::noncopyable
{
public:
    informer();
    ~informer();

    void start();
    void stop();

    void set_credentials(credentials&& credentials);
    void reset_credentials();

    void set_instance_id(instance_id const& instance_id);
    void set_completion_handler(query::handler const& handler);
    void set_proxy_settings(net::http::proxy_settings const& proxy_settings);

    void add_query(query::pointer query);

private:
    void start_worker_threads();
    void stop_worker_threads();

    void stop_session();

    void start_available_queries();
    void start_query(query::pointer query);

    void add_common_queries();
    void remove_all_queries();
    void cancel_custom_queries();

    void cancel_all_timers();
    void start_error_timer(int timeout);
    void start_timer(info::type info, int timeout);

    std::string encrypt(std::string const& data);
    std::string decrypt(std::string const& data);

    void append_auth_data(missio::json::object& json_data);
    net::http::request create_request(std::string const& data);
    net::http::request create_info_request(query::pointer query);

    error::type process_query_response(query::pointer query);
    void process_response_data(missio::json::object const& json_data);

    void handle_info(query::pointer query, boost::system::error_code const& error);
    void handle_query_timer(info::type info, boost::system::error_code const& error);
    void handle_error_timer(boost::system::error_code const& error);

    void call_handler(query::pointer query, error::type error);

private:
    bool online_;
    bool stopped_;
    bool processing_;

    boost::mutex mutex_;

    instance_id instance_id_;
    std::string informer_id_;

    crypto::manager crypto_manager_;

    boost::asio::io_service io_service_;
    net::util::thread_pool io_threads_;

    query::handler handler_;
    //query_queue queries_;

    credentials credentials_;

    net::util::async_timer query_timer_;
    net::util::async_timer error_timer_;
    net::util::async_timer pending_timer_;

    net::http::client_session::pointer session_;
};

}   // namespace chat

#endif  // _chat_core_informer_hpp
