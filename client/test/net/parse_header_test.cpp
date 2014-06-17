
// Application headers
#include <net/http/detail/header_grammar.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(parse_header_test_suite)

// Helper parsing function
bool parse_header(std::string const& string, http::header& header)
{
    using namespace boost::spirit::qi;

    typedef std::string::const_iterator const_iterator;
    static http::detail::header_grammar<const_iterator> const context;

    std::string::const_iterator first = string.begin();
    std::string::const_iterator last = string.end();

    bool result = qi::parse(first, last, context, header);

    if(result)
    {
        if(first != last)
            result = false;
    }

    return result;
}

BOOST_AUTO_TEST_CASE(parse_valid_header_test)
{
    http::header header;

    BOOST_REQUIRE(parse_header("Host: www.host.com\r\n", header));
    BOOST_CHECK_EQUAL(header.name, "Host");
    BOOST_CHECK_EQUAL(header.value, "www.host.com");

    BOOST_REQUIRE(parse_header("User-Agent: Mozilla/5.0 (Windows NT 5.1) Chrome/12.0.742.100\r\n", header));
    BOOST_CHECK_EQUAL(header.name, "User-Agent");
    BOOST_CHECK_EQUAL(header.value, "Mozilla/5.0 (Windows NT 5.1) Chrome/12.0.742.100");

    BOOST_REQUIRE(parse_header("Multi-Line-Header: This is a header\r\n"
                               " that spans multiple lines and you can see\r\n"
                               "\tit is being processed properly\r\n", header));
    BOOST_CHECK_EQUAL(header.name, "Multi-Line-Header");
    BOOST_CHECK_EQUAL(header.value, "This is a header that spans multiple lines and you can see it is being processed properly");
}

BOOST_AUTO_TEST_CASE(parse_invalid_header_test)
{
    http::header header;

    BOOST_CHECK_EQUAL(parse_header("", header), false);
    BOOST_CHECK_EQUAL(parse_header("\r\n", header), false);
    BOOST_CHECK_EQUAL(parse_header("Host\r\n", header), false);
    BOOST_CHECK_EQUAL(parse_header("With Space:\r\n", header), false);
    BOOST_CHECK_EQUAL(parse_header("Without-Value:\r\n", header), false);
    BOOST_CHECK_EQUAL(parse_header("Accept: without-new-line", header), false);
}

BOOST_AUTO_TEST_SUITE_END()
