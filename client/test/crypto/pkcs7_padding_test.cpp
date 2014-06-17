//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//--------------------------------------------------------------------------- 

// Application headers
#include <crypto/pkcs7/padding.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(pkcs7_padding_test_suite)

BOOST_AUTO_TEST_CASE(add_padding_test)
{
    unsigned char input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    unsigned char expected[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06
    };

    crypto::pkcs7::add_padding(input, 16, 10);

    BOOST_CHECK_EQUAL_COLLECTIONS(input, input + 16, expected, expected + 16);
}

BOOST_AUTO_TEST_CASE(add_padding_for_empty_block_test)
{
    unsigned char input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    unsigned char expected[16] =
    {
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
    };

    crypto::pkcs7::add_padding(input, 16, 0);

    BOOST_CHECK_EQUAL_COLLECTIONS(input, input + 16, expected, expected + 16);
}

BOOST_AUTO_TEST_CASE(add_padding_for_invalid_data_size)
{
    unsigned char input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    BOOST_CHECK_THROW(crypto::pkcs7::add_padding(input, 16, 20), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(remove_padding_test)
{
    unsigned char input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x04, 0x04, 0x04, 0x04
    };

    BOOST_CHECK_EQUAL(crypto::pkcs7::remove_padding(input, 16), 4);
}

BOOST_AUTO_TEST_CASE(remove_padding_from_invalid_data_test)
{
    unsigned char input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x07, 0x07, 0x07, 0x07, 0x07
    };

    BOOST_CHECK_THROW(crypto::pkcs7::remove_padding(input, 16), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(remove_padding_of_invalid_size_test)
{
    unsigned char input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0xEE, 0xEE, 0xEE, 0xEE
    };

    BOOST_CHECK_THROW(crypto::pkcs7::remove_padding(input, 16), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
