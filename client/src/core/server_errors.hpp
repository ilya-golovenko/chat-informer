//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_core_server_errors_hpp
#define _chat_core_server_errors_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200) 


namespace chat
{
namespace server_error
{

typedef int type;

type const db_connect       = 1;
type const db_query         = 2;

type const id_banned        = 100;
type const need_id          = 101;

type const bad_credentials  = 200;
type const send_denied      = 201;
type const should_wait      = 202;
type const ignored          = 203;
type const offline          = 204;
type const generic          = 205;

}   // namespace server_error
}   // namespace chat

#endif  // _chat_core_server_errors_hpp
