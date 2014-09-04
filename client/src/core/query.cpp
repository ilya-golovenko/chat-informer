//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/query.hpp>

// Windows headers
#include <windows.h>

// BOOST headers
#include <boost/make_shared.hpp>


namespace chat
{

query::pointer query::create(missio::json::value const& data, bool need_auth, handler const& handler)
{
    return boost::make_shared<query>(data, need_auth, handler);
}

query::query(missio::json::value const& data, bool need_auth, handler const& handler) :
    error_(error::unknown),
    data_(data),
    need_auth_(need_auth),
    handler_(handler),
    completed_(0)
{
}

void query::cancel()
{
    if(::InterlockedExchange(&completed_, 1) == 0)
        error_ = error::operation_aborted;
}

bool query::is_completed()
{
    return 1 == ::InterlockedExchangeAdd(&completed_, 0);
}

bool query::need_auth() const
{
    return need_auth_;
}

error::type query::error() const
{
    return error_;
}

json::value& query::json_data()
{
    return data_;
}

json::value const& query::json_data() const
{
    return data_;
}

void query::set_json_data(missio::json::value const& data)
{
    data_.assign(data);
}

void query::handle_info(error::type error)
{
    if(::InterlockedExchange(&completed_, 1) == 0)
    {
        error_ = error;
        handler_(shared_from_this());
    }
}

}   // namespace chat
