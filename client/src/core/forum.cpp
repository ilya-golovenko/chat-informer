//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/forum.hpp>


namespace chat
{

forum_topic_list const& forum::topics() const
{
    return topics_;
}

bool forum::update(missio::json::object const& json_data)
{
    if(json_data.contains("forum"))
    {
        return topics_.update(json_data["forum"]);
    }

    return false;
}

}   // namespace chat
