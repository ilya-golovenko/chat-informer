//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/query_queue.hpp>

// BOOST headers
#include <boost/range/algorithm.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

// STL headers
#include <algorithm>

/*
namespace missio
{

query_queue::pointer query_queue::create(boost::asio::io_service& io_service,
                                         handler const& handler)
{
    return boost::make_shared<query_queue>(boost::ref(io_service), handler);
}

query_queue::query_queue(boost::asio::io_service& io_service,
                         handler const& handler) :
    handler_(handler),
    timer_(io_service_)
{
}

query_queue::~query_queue()
{
    clear();
}

void query_queue::add(query::pointer query)
{
    queries_.push_back(query);

    boost::sort(queries_, boost::bind(&query::time, _1) < boost::bind(&query::time, _2));

    //TODO: remove duplicate entries

    std::time_t time = queries_.begin()->time();

    timer_.start(boost::posix_time::seconds(time), boost::bind(
        &query_queue::handle_timer, shared_from_this(), _1));
}

bool query_queue::empty() const
{
    return queries_.empty();
}

void query_queue::clear()
{
    queries_.clear();
    timer_.stop();
}

void query_queue::handle_timer(boost::system::error_code const& error)
{
    if(!error)
    {
        if(!queries_.empty())
        {
            query_vector queries;

            queries.push_back(queries_.front());
            queries.erase(queries_.begin());

            std::time_t time = queries.front().time();

            while(!queries_.empty())
            {
                if(queries_.front().time() - time <= 10)
                {
                    queries.push_back(queries_.front());
                    queries.erase(queries_.begin());

                    continue;
                }

                break;
            }

            if(!queries_.empty())
            {
                time = queries_.front().time();

                timer_.start(boost::posix_time::seconds(time), boost::bind(
                    &query_queue::handle_timer, shared_from_this(), _1));
            }

            try
            {
                handler_(queries);
            }
            catch(std::exception const& e)
            {
                LOG_ERROR("caught exception: ", e);
            }
        }
    }
}

}   // namespace missio
*/