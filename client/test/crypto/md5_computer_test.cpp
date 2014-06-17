//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//--------------------------------------------------------------------------- 

// Application headers
#include <crypto/md5/computer.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(md5_computer_test_suite)

BOOST_AUTO_TEST_CASE(md5_encode_empty_string_test)
{
    char const input[] = "";

    unsigned char const expected[16] =
    {
        0xD4, 0x1D, 0x8C, 0xD9, 0x8F, 0x00, 0xB2, 0x04,
        0xE9, 0x80, 0x09, 0x98, 0xEC, 0xF8, 0x42, 0x7E
    };

    crypto::md5::computer md5_computer(input, std::strlen(input));
    crypto::md5::digest md5_digest = md5_computer.get_digest();

    unsigned char output[16];
    md5_digest.copy(output);

    BOOST_CHECK_EQUAL_COLLECTIONS(output, output + sizeof(output), expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(md5_encode_alnum_string_test)
{
    char const input[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    unsigned char const expected[16] =
    {
       0xD1, 0x74, 0xAB, 0x98, 0xD2, 0x77, 0xD9, 0xF5,
       0xA5, 0x61, 0x1C, 0x2C, 0x9F, 0x41, 0x9D, 0x9F
    };

    crypto::md5::computer md5_computer(input, std::strlen(input));
    crypto::md5::digest md5_digest = md5_computer.get_digest();

    unsigned char output[16];
    md5_digest.copy(output);

    BOOST_CHECK_EQUAL_COLLECTIONS(output, output + sizeof(output), expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(md5_encode_digest_to_string_test)
{
    char const input[] = "abcdefghijklmnopqrstuvwxyz";
    std::string expected = "C3FCD3D76192E4007DFB496CCA67E13B";

    crypto::md5::computer md5_computer(input, std::strlen(input));
    std::string output = md5_computer.get_digest().to_string();

    BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(md5_encode_append_string_test)
{
    char const input1[] = "message";
    char const input2[] = " digest";

    std::string expected = "F96B697D7CB7938D525A2F31AAF161D0";

    crypto::md5::computer md5_computer;
    md5_computer.append(input1, std::strlen(input1));
    md5_computer.append(input2, std::strlen(input2));
    std::string output = md5_computer.get_digest().to_string();

    BOOST_CHECK_EQUAL_COLLECTIONS(output.begin(), output.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_SUITE_END()
