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


namespace chat
{

query::pointer query::create(missio::json::value const& data, bool need_auth, handler const& handler)
{
    return std::make_shared<query>(data, need_auth, handler);
}

query::query(missio::json::value const& data, bool need_auth, handler const& handler) :
    error_(error::unknown),
    data_(data),
    need_auth_(need_auth),
    handler_(handler),
    completed_(false)
{
}

void query::cancel()
{
    if(!completed_.exchange(true))
        error_ = error::operation_aborted;
}

bool query::is_completed()
{
    return completed_;
}

bool query::need_auth() const
{
    return need_auth_;
}

error::type query::error() const
{
    return error_;
}

missio::json::value& query::json_data()
{
    return data_;
}

missio::json::value const& query::json_data() const
{
    return data_;
}

void query::set_json_data(missio::json::value const& data)
{
    data_ = data;
}

void query::handle_info(error::type error)
{
    if(!completed_.exchange(true))
    {
        error_ = error;
        handler_(shared_from_this());
    }
}

}   // namespace chat
