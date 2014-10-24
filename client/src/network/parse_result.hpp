//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_parse_result_hpp
#define _chat_network_parse_result_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)


namespace chat
{
namespace net
{

enum class parse_result
{
  ok,
  more,
  failure
};

}   // namespace net
}   // namespace chat

#endif  // _chat_network_parse_result_hpp
