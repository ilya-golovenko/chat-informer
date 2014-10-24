//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <network/http/client_session.hpp>
#include <network/http/uri_builder.hpp>
#include <network/util/bind_handler.hpp>
#include <network/util/statistics.hpp>

// BOOST headers
#include <boost/algorithm/string.hpp>

// STL headers
#include <utility>


namespace chat
{
namespace net
{
namespace http
{

client_session::pointer client_session::create(boost::asio::io_service& io_service)
{
    return std::make_shared<client_session>(std::ref(io_service));
}

client_session::client_session(boost::asio::io_service& io_service) :
    basic_session(tcp::connection(io_service)),
    port_(80u),
    redirect_count_(0u),
    resolver_(io_service)
{
}

void client_session::set_proxy_settings(proxy_settings const& proxy_settings)
{
    proxy_settings_ = proxy_settings;
}

void client_session::write_request(std::string const& host, std::uint16_t port, request&& request)
{
    request_ = std::move(request);
    open_session(host, port, false);
}

void client_session::write_request(uri const& uri, request&& request)
{
    request_ = std::move(request);

    request.set_uri(uri.relative());
    request.set_host(uri.host_info());

    write_request(uri.host(), uri.port(), false);
}

void client_session::read_response()
{
    begin_read_message();
}

void client_session::open_session(std::string const& host, std::uint16_t port, bool redirected)
{
    std::string new_host = host;
    std::uint16_t new_port = port;

    if(proxy_settings_.use_proxy())
    {
        new_host = proxy_settings_.get_host();
        new_port = proxy_settings_.get_port();

        uri_builder uri_builder(request_.get_uri());

        uri_builder.set_host(host_);
        uri_builder.set_port(port_);

        request_.set_uri(uri_builder.uri());
    }

    if(!redirected)
        redirect_count_ = 0u;

    if(connection().is_open()
        && is_keep_alive()
        && new_host == host_
        && new_port == port_)
    {
        LOG_COMP_DEBUG(client_session, "using existing session");
        begin_write_message(request_);
    }
    else
    {
        close(true);

        port_ = new_port;
        host_ = new_host;

        begin_resolve(host_, port_);
    }
}

void client_session::restart_session()
{
    LOG_COMP_DEBUG(client_session, "restarting session");

    close(true);

    begin_resolve(host_, port_);
}

bool client_session::follow_redirect()
{
    if(response_.get_status().is_redirected())
    {
        if(++redirect_count_ < 5u)
        {
            boost::optional<std::string> value = response_.get(header_names::location);

            if(value)
            {
                uri const uri(*value);

                request_.set_uri(uri.relative());

                LOG_COMP_DEBUG(client_session, "redirecting to the uri: ", uri);
                open_session(uri.host(), uri.port(), true);

                return true;
            }
        }
    }

    return false;
}

bool client_session::auth_proxy_request()
{
    if(proxy_settings_.use_proxy())
    {
        if(response_.get_status() == status::proxy_authentication_required)
        {
            LOG_COMP_DEBUG(client_session, "http proxy server requires authentication");

            std::vector<std::string> schemas = response_.get_range(header_names::proxy_authenticate);

            if(!schemas.empty())
            {
                try
                {
                    //if(!proxy_authorization.empty())
                    //{
                        //LOG_COMP_DEBUG(client_session, "adding authorization information to http request");
                        //request_.set(header_names::proxy_authorization, proxy_authorization);

                        //open_session(host_, port_, false);

                        //return true;
                    //}
                }
                catch(std::exception const& e)
                {
                    LOG_COMP_WARNING(client_session, e);
                }
            }
        }
    }

    return false;
}

bool client_session::is_keep_alive_error(boost::system::error_code const& error) const
{
    return error == boost::asio::error::eof ||
           error == boost::asio::error::bad_descriptor ||
           error == boost::asio::error::connection_reset ||
           error == boost::asio::error::connection_aborted;
}

bool client_session::is_keep_alive_aborted(boost::system::error_code const& error) const
{
    return is_keep_alive() && is_keep_alive_error(error);
}

void client_session::begin_resolve(std::string const& host, std::uint16_t port)
{
    LOG_COMP_DEBUG(client_session, "resolving http server hostname: ", host);

    boost::asio::ip::tcp::resolver::query query(host, std::to_string(port),
        boost::asio::ip::tcp::resolver::query::numeric_service);

    resolver_.async_resolve(query, bind_handler(shared_from_this(), &client_session::handle_resolve));
}

void client_session::begin_connect(boost::asio::ip::tcp::resolver::iterator iterator)
{
    LOG_COMP_DEBUG(client_session, "connecting to http server: ", iterator->host_name());

    boost::asio::async_connect(get_tcp_connection(), iterator, bind_handler(shared_from_this(), &client_session::handle_connect));
}

void client_session::on_message_read(response const& response)
{
    response_ = response;

    session_base::on_message_read(response);
}

void client_session::on_completed(boost::system::error_code const& error)
{
    if(is_keep_alive_aborted(error))
    {
        restart_session();
        return;
    }

    if(!error)
    {
        if(follow_redirect())
        {
            return;
        }

        if(auth_proxy_request())
        {
            return;
        }
    }

    session_base::on_completed(error);
}

void client_session::handle_resolve(boost::system::error_code const& error, boost::asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
    {
        LOG_COMP_DEBUG(client_session, "successfully resolved http server hostname");
        begin_connect(iterator);
    }
    else
    {
        LOG_COMP_DEBUG(client_session, "cannot resolve http server hostname: ", error);
        on_completed(error);
    }
}

void client_session::handle_connect(boost::system::error_code const& error, boost::asio::ip::tcp::resolver::iterator iterator)
{
    if(!error)
    {
        LOG_COMP_DEBUG(client_session, "successfully connected to http server: ", iterator->endpoint());
        begin_write_message(request_);
    }
    else
    {
        LOG_COMP_DEBUG(client_session, "cannot connect to http server: ", error);
        on_completed(error);
    }
}
/*
authentication::pointer client_session::create_proxy_auth(std::vector<std::string> const& schemas)
{
    authentication::pointer authetication;

    for(std::size_t i = 0; i < schemas.size() && !authetication; ++i)
    {
        //proxy_auth = authentication_factory::create(schemas[i], 
    }

    return authetication;
}
*/
}   // namespace http
}   // namespace net
}   // namespace chat
