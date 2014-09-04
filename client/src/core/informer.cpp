//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/common.hpp>
#include <core/informer.hpp>
#include <core/exception.hpp>
#include <core/errors.hpp>
#include <core/factory.hpp>
#include <crypto/common.hpp>
#include <utility/make_hex_string.hpp>

// MISSIO headers
#include <missio/logging/common.hpp>
#include <missio/json/json_io.hpp>

// BOOST headers
#include <boost/bind.hpp>


namespace chat
{

informer::informer() :
    online_(false),
    stopped_(true),
    processing_(false),
    io_threads_(io_service_),
    query_timer_(io_service_),
    error_timer_(io_service_),
    pending_timer_(io_service_)
{
    crypto_manager_.set_uuid_key(missio::crypto_key);

    session_ = net::http::client_session::create(io_service_);
    informer_id_ = util::make_hex_string(missio::informer_id.data, 16);
}

informer::~informer()
{
    //TODO: cancel all timers
    io_threads_.stop();
    session_.reset();
}

void informer::start()
{
    boost::mutex::scoped_lock lock(mutex_);

    if(stopped_)
    {
        LOG_INFO("starting");

        stopped_ = false;
        io_threads_.start(1);

        add_common_queries();
    }
}

void informer::stop()
{
    boost::mutex::scoped_lock lock(mutex_);

    if(!stopped_)
    {
        LOG_INFO("stopping");

        stopped_ = true;
        io_threads_.stop();

        stop_session();
        cancel_all_timers();
        cancel_custom_queries();
    }
}

void informer::set_credentials(credentials&& credentials)
{
    boost::mutex::scoped_lock lock(mutex_);

    LOG_DEBUG("setting credentials");
    credentials_ = std::move(credentials);

    if(!stopped_)
    {
        //queries_.push(info::auth);
        //start_available_queries();
    }
}

void informer::reset_credentials()
{
    boost::mutex::scoped_lock lock(mutex_);

    LOG_DEBUG("resetting credentials");
    //credentials_.clear();
}

void informer::set_instance_id(instance_id const& instance_id)
{
    boost::mutex::scoped_lock lock(mutex_);

    LOG_DEBUG("setting instance ID: ", instance_id);
    instance_id_ = instance_id;
}

void informer::set_completion_handler(query::handler const& handler)
{
    boost::mutex::scoped_lock lock(mutex_);

    LOG_DEBUG("setting completion handler");
    handler_ = handler;
}

void informer::set_proxy_settings(net::http::proxy_settings const& proxy_settings)
{
    boost::mutex::scoped_lock lock(mutex_);

    if(stopped_)
    {
        LOG_DEBUG("setting proxy settings");
        session_->set_proxy_settings(proxy_settings);
    }
}

void informer::add_query(query::pointer query)
{
    boost::mutex::scoped_lock lock(mutex_);

    if(!stopped_)
    {
        if(online_)
        {
            //queries_.push(query);
            //start_available_queries();
        }
        else
        {
            call_handler(query, error::network_io_error);
        }
    }
}

void informer::stop_session()
{
    if(session_)
    {
        if(session_->is_open())
        {
            LOG_DEBUG("stopping session");
            session_->close(true);
        }
    }
}

void informer::start_available_queries()
{
    if(!processing_)
    {
        query::pointer query;
/*
        if(!queries_.empty())
        {
            missio::json::value data;
            bool need_auth = false;

            info_set::iterator begin = queries_.begin();
            info_set::iterator end = queries_.end();

            for(info_set::iterator it = begin; it != end; ++it)
            {
                if(info::auth == *it)
                    need_auth = true;

                data[info::to_string(*it)] = true;
            }

            query = query::create(data, need_auth, handler_);

            queries_.clear();
        }
        else
        {
            while(!custom_queries_.empty())
            {
                if(custom_queries_.front()->is_completed())
                {
                    custom_queries_.pop();
                    continue;
                }

                break;
            }

            if(!custom_queries_.empty())
            {
                query = custom_queries_.front();
                custom_queries_.pop();
            }
        }

        if(query)
            start_query(query);
*/
    }
}

void informer::start_query(query::pointer query)
{
    if(session_)
    {
        processing_ = true;

        net::http::request request = create_info_request(query);

        //session_->start(informer_hostname, net::http::http_port_number,
        //    request, boost::bind(&informer::handle_info, this, query, _1));
    }
}

void informer::add_common_queries()
{
    LOG_DEBUG("adding common queries");
    //queries_.push_common_queries();
    //start_next_query();
}

void informer::remove_all_queries()
{
    LOG_DEBUG("removing all queries");
    //queries_.clear();
}

void informer::cancel_custom_queries()
{
    LOG_DEBUG("cancelling custom queries");
}

void informer::cancel_all_timers()
{
    LOG_DEBUG("cancelling all timers");
}

void informer::start_error_timer(int timeout)
{
    LOG_DEBUG("starting error timer: ", timeout, " seconds");
}

void informer::start_timer(info::type info, int timeout)
{
    LOG_DEBUG("starting timer: ", info, ", ", timeout, " seconds");
}

std::string informer::encrypt(std::string const& data)
{
    return crypto::encrypt(data, crypto_manager_);
}

std::string informer::decrypt(std::string const& data)
{
    return crypto::decrypt(data, crypto_manager_);
}

void informer::append_auth_data(missio::json::object& json_data)
{
    json_data["auth_data"]["instance_id"] = instance_id_.to_string();
    json_data["auth_data"]["version"] = informer_version.to_string();

    //if(!credentials_.empty())
    //    json_data["auth_data"]["credentials"] = credentials_.to_string();
}

net::http::request informer::create_request(std::string const& data)
{
    net::http::request request;

    request.assign_content(data);
    request.set_content_length_header();

    request.set_user_agent(informer_name);
    request.set_host(informer_hostname);
    request.set_uri(informer_pub_path);

    request.set_version(net::http::version(1, 1));
    request.set_method(net::http::request_methods::post);
    request.set_connection(net::http::connection_tokens::keep_alive);

    request.add(net::http::header_names::pragma, net::http::cache_tokens::no_cache);
    request.add(net::http::header_names::cache_control, net::http::cache_tokens::no_cache);

    return request;
}

net::http::request informer::create_info_request(query::pointer query)
{
    if(query->need_auth())
        append_auth_data(query->json_data());

    std::string data = missio::json::write(query->json_data());
    net::http::request request = create_request(encrypt(data));

    request.add("X-Chat-Informer-ID", informer_id_);
    request.set_content_type("application/x-encrypted");

    return request;
}

error::type informer::process_query_response(query::pointer query)
{
    /*net::http::response const& response = session_->response();

    if(!response.status().is_successful())
        return error::bad_http_response;

    boost::optional<std::string> value = response.get_content_type();

    if(!value || !boost::algorithm::iequals(*value, "application/x-encrypted"))
        return error::bad_http_response;

    std::string data = decrypt(response.content());
    missio::json::value json_data = missio::json::read(data);

    if(!json_data["server_status"].as<bool>())
        return error::bad_server_status;

    process_response_data(json_data);
    query->set_json_data(json_data);
*/
    return error::successful;
}

void informer::process_response_data(missio::json::object const& json_data)
{
    if(json_data.contains("refresh"))
    {
        missio::json::object const& event_refresh = json_data["refresh"];
/*
        for(info::type info = info::type_first;
            info <= info::type_last; ++info)
        {
            char const* name = info::to_string(info);

            if(event_refresh.contains(name))
                start_timer(info, event_refresh[name]);
        }
*/
    }
}

void informer::handle_info(query::pointer query, boost::system::error_code const& error)
{
    boost::mutex::scoped_lock lock(mutex_);

    processing_ = false;

    if(!stopped_)
    {
        error::type query_error = error::network_io_error;

        if(!error)
        {
            try
            {
                query_error = process_query_response(query);
            }
            catch(missio::json::exception const& e)
            {
                LOG_ERROR("caught exception: ", e);
                query_error = error::bad_server_data;
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: ", e);
                query_error = error::unknown;
            }
        }

        if(!query_error)
        {
            online_ = true;
            //start_next_query();
        }
        else
        {
            online_ = false;
            cancel_all_timers();
            start_error_timer(30);
            cancel_custom_queries();
        }

        call_handler(query, query_error);
    }
}

void informer::handle_query_timer(info::type info, boost::system::error_code const& error)
{
    if(!error)
    {
        boost::mutex::scoped_lock lock(mutex_);

        if(!stopped_)
        {
            //queries_.insert(info);
            start_available_queries();
        }
    }
}

void informer::handle_error_timer(boost::system::error_code const& error)
{
    if(!error)
    {
        boost::mutex::scoped_lock lock(mutex_);

        if(!stopped_)
            add_common_queries();
    }
}

void informer::call_handler(query::pointer query, error::type error)
{
    if(!query->is_completed())
    {
        //TODO: use signal here
        //missio::factory::dispatcher().dispatch_handler(
        //    boost::bind(&query::handle_info, query, error));
    }
}

}   // namespace chat
