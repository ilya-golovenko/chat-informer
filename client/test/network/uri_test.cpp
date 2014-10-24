//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//--------------------------------------------------------------------------- 

// Application headers
#include <network/uri/impl/uri_grammar.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>

// STL headers
#include <string>


BOOST_AUTO_TEST_SUITE(uri_test_suite)

struct uri_fixture
{
    static bool parse_uri(chat::net::impl::uri_parts& uri_parts, std::string const& str)
    {
        static chat::net::impl::uri_grammar<std::string::const_iterator> const uri_grammar;
        return boost::spirit::qi::parse(std::begin(str), std::end(str), uri_grammar, uri_parts);
    }
};

BOOST_FIXTURE_TEST_CASE(parse_hierarchical_uri_1_test, uri_fixture)
{
    chat::net::impl::uri_parts uri_parts;

    BOOST_CHECK(parse_uri(uri_parts, "foo://username:password@example.com:8042/over/there/index.dtb?type=animal&name=narwhal#nose"));

    BOOST_REQUIRE_EQUAL(!uri_parts.scheme, false);
    BOOST_CHECK_EQUAL(uri_parts.scheme.get(), "foo");

    BOOST_REQUIRE_EQUAL(!uri_parts.user_info, false);
    BOOST_CHECK_EQUAL(uri_parts.user_info.get(), "username:password");

    BOOST_REQUIRE_EQUAL(!uri_parts.host, false);
    BOOST_CHECK_EQUAL(uri_parts.host.get(), "example.com");

    BOOST_REQUIRE_EQUAL(!uri_parts.port, false);
    BOOST_CHECK_EQUAL(uri_parts.port.get(), 8042u);

    BOOST_REQUIRE_EQUAL(!uri_parts.path, false);
    BOOST_CHECK_EQUAL(uri_parts.path.get(), "/over/there/index.dtb");

    BOOST_REQUIRE_EQUAL(!uri_parts.query, false);
    BOOST_CHECK_EQUAL(uri_parts.query.get(), "type=animal&name=narwhal");

    BOOST_REQUIRE_EQUAL(!uri_parts.fragment, false);
    BOOST_CHECK_EQUAL(uri_parts.fragment.get(), "nose");
}

BOOST_FIXTURE_TEST_CASE(parse_hierarchical_uri_2_test, uri_fixture)
{
    chat::net::impl::uri_parts uri_parts;

    BOOST_CHECK(parse_uri(uri_parts, "http://www.example.com/glynos/?key=value#frag"));

    BOOST_REQUIRE_EQUAL(!uri_parts.scheme, false);
    BOOST_CHECK_EQUAL(uri_parts.scheme.get(), "http");

    BOOST_CHECK_EQUAL(!uri_parts.user_info, true);

    BOOST_REQUIRE_EQUAL(!uri_parts.host, false);
    BOOST_CHECK_EQUAL(uri_parts.host.get(), "www.example.com");

    BOOST_CHECK_EQUAL(!uri_parts.port, true);

    BOOST_REQUIRE_EQUAL(!uri_parts.path, false);
    BOOST_CHECK_EQUAL(uri_parts.path.get(), "/glynos/");

    BOOST_REQUIRE_EQUAL(!uri_parts.query, false);
    BOOST_CHECK_EQUAL(uri_parts.query.get(), "key=value");

    BOOST_REQUIRE_EQUAL(!uri_parts.fragment, false);
    BOOST_CHECK_EQUAL(uri_parts.fragment.get(), "frag");
}

BOOST_FIXTURE_TEST_CASE(parse_opaque_uri_1_test, uri_fixture)
{
    chat::net::impl::uri_parts uri_parts;

    BOOST_CHECK(parse_uri(uri_parts, "xmpp:example-node@example.com?message;subject=Hello%20World"));

    BOOST_REQUIRE_EQUAL(!uri_parts.scheme, false);
    BOOST_CHECK_EQUAL(uri_parts.scheme.get(), "xmpp");

    BOOST_CHECK_EQUAL(!uri_parts.user_info, true);

    BOOST_CHECK_EQUAL(!uri_parts.host, true);

    BOOST_CHECK_EQUAL(!uri_parts.port, true);

    BOOST_REQUIRE_EQUAL(!uri_parts.path, false);
    BOOST_CHECK_EQUAL(uri_parts.path.get(), "example-node@example.com");

    BOOST_REQUIRE_EQUAL(!uri_parts.query, false);
    BOOST_CHECK_EQUAL(uri_parts.query.get(), "message;subject=Hello World");

    BOOST_CHECK_EQUAL(!uri_parts.fragment, true);
}

BOOST_FIXTURE_TEST_CASE(parse_opaque_uri_2_test, uri_fixture)
{
    chat::net::impl::uri_parts uri_parts;

    BOOST_CHECK(parse_uri(uri_parts, "mailto:username@example.com?subject=Topic"));

    BOOST_REQUIRE_EQUAL(!uri_parts.scheme, false);
    BOOST_CHECK_EQUAL(uri_parts.scheme.get(), "mailto");

    BOOST_CHECK_EQUAL(!uri_parts.user_info, true);

    BOOST_CHECK_EQUAL(!uri_parts.host, true);

    BOOST_CHECK_EQUAL(!uri_parts.port, true);

    BOOST_REQUIRE_EQUAL(!uri_parts.path, false);
    BOOST_CHECK_EQUAL(uri_parts.path.get(), "username@example.com");

    BOOST_REQUIRE_EQUAL(!uri_parts.query, false);
    BOOST_CHECK_EQUAL(uri_parts.query.get(), "subject=Topic");

    BOOST_CHECK_EQUAL(!uri_parts.fragment, true);
}

BOOST_FIXTURE_TEST_CASE(parse_file_uri_test, uri_fixture)
{
    chat::net::impl::uri_parts uri_parts;

    BOOST_CHECK(parse_uri(uri_parts, "file:///etc/fstab"));

    BOOST_REQUIRE_EQUAL(!uri_parts.scheme, false);
    BOOST_CHECK_EQUAL(uri_parts.scheme.get(), "file");

    BOOST_CHECK_EQUAL(!uri_parts.user_info, true);

    BOOST_REQUIRE_EQUAL(!uri_parts.host, false);
    BOOST_CHECK_EQUAL(uri_parts.host.get(), "");

    BOOST_CHECK_EQUAL(!uri_parts.port, true);

    BOOST_REQUIRE_EQUAL(!uri_parts.path, false);
    BOOST_CHECK_EQUAL(uri_parts.path.get(), "/etc/fstab");

    BOOST_REQUIRE_EQUAL(!uri_parts.query, true);

    BOOST_CHECK_EQUAL(!uri_parts.fragment, true);
}

BOOST_AUTO_TEST_SUITE_END()
