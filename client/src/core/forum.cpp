//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <core/forum.hpp>


namespace missio
{

forum::forum()
{
}

forum::~forum()
{
}

forum_topic_list const& forum::topics() const
{
    return topics_;
}

bool forum::update(json::object_cref json_data)
{
    if(json_data->contains("forum"))
        return topics_.update(json_data["forum"]);

    return false;
}

}   // namespace missio
