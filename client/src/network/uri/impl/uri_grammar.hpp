//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2009, 2011, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_network_uri_impl_uri_grammar_hpp
#define _chat_network_uri_impl_uri_grammar_hpp

// Application headers
#include <network/uri/impl/uri_parts.hpp>

// BOOST headers
#include <boost/fusion/include/adapt_struct_named.hpp>
#include <boost/spirit/include/qi.hpp>


BOOST_FUSION_ADAPT_STRUCT_NAMED_NS
(
    chat::net::impl::uri_parts,
    (chat)(net)(impl),
    hierarchical_uri,

    (boost::optional<std::string>, scheme)
    (boost::optional<std::string>, user_info)
    (boost::optional<std::string>, host)
    (boost::optional<std::uint16_t>, port)
    (boost::optional<std::string>, path)
    (boost::optional<std::string>, query)
    (boost::optional<std::string>, fragment)
)

BOOST_FUSION_ADAPT_STRUCT_NAMED_NS
(
    chat::net::impl::uri_parts,
    (chat)(net)(impl),
    opaque_uri,

    (boost::optional<std::string>, scheme)
    (boost::optional<std::string>, path)
    (boost::optional<std::string>, query)
    (boost::optional<std::string>, fragment)
)


namespace chat
{
namespace net
{
namespace impl
{

template <typename Iterator>
struct uri_grammar : boost::spirit::qi::grammar<Iterator, uri_parts()>
{
    uri_grammar() : uri_grammar::base_type(uri_)
    {
        using namespace boost::spirit::qi;

        uint_parser<std::uint8_t, 16, 2, 2> encoded_parser;
        uint_parser<std::uint16_t, 10, 1, 5> port_parser;

        alpha_              =   no_case[char_('a', 'z')];

        digit_              =   char_('0', '9');

        alnum_              =   alpha_ | digit_;

        pct_encoded_        =   '%' >> encoded_parser;

        unreserved_         =   alnum_ | char_("-._~");

        sub_delims_         =   char_("!$&'()*+,;=");

        pchar_              =   unreserved_ | pct_encoded_ | sub_delims_ | char_(":@");

        scheme_             =   alpha_ >> *(alnum_ | char_("+.-")) >> ':';

        user_info_          =   +(unreserved_ | pct_encoded_ | sub_delims_ | char_(':')) >> '@';

        host_               =   *(unreserved_ | pct_encoded_ | sub_delims_ | char_("[]"));

        port_               =   ':' >> port_parser;

        path_               =   *(pchar_ | char_('/'));

        query_              =   '?' >> *(pchar_ | char_("/?"));

        fragment_           =   '#' >> *(pchar_ | char_("/?"));

        hierarchical_uri_   =   -scheme_ >> "//" >> -user_info_ >> host_ >> -port_ >> path_ >> -query_ >> -fragment_;

        opaque_uri_         =   -scheme_ >> path_ >> -query_ >> -fragment_;

        uri_                =   hold[hierarchical_uri_] | opaque_uri_;
    }

    boost::spirit::qi::rule<Iterator, char()> alpha_;
    boost::spirit::qi::rule<Iterator, char()> digit_;
    boost::spirit::qi::rule<Iterator, char()> alnum_;
    boost::spirit::qi::rule<Iterator, char()> pct_encoded_;
    boost::spirit::qi::rule<Iterator, char()> unreserved_;
    boost::spirit::qi::rule<Iterator, char()> sub_delims_;
    boost::spirit::qi::rule<Iterator, char()> pchar_;

    boost::spirit::qi::rule<Iterator, std::string()> scheme_;
    boost::spirit::qi::rule<Iterator, std::string()> user_info_;
    boost::spirit::qi::rule<Iterator, std::string()> host_;
    boost::spirit::qi::rule<Iterator, std::uint16_t()> port_;
    boost::spirit::qi::rule<Iterator, std::string()> path_;
    boost::spirit::qi::rule<Iterator, std::string()> query_;
    boost::spirit::qi::rule<Iterator, std::string()> fragment_;

    boost::spirit::qi::rule<Iterator, hierarchical_uri()> hierarchical_uri_;
    boost::spirit::qi::rule<Iterator, opaque_uri()> opaque_uri_;

    boost::spirit::qi::rule<Iterator, uri_parts()> uri_;
};

}   // namespace impl
}   // namespace net
}   // namespace chat

#endif  // _chat_network_uri_impl_uri_grammar_hpp
