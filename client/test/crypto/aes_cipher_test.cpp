//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------

// Application headers
#include <crypto/aes/cipher.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(aes_cipher_test_suite)

BOOST_AUTO_TEST_CASE(aes_cipher_encryption_test)
{
    unsigned char const crypto_key[16] =
    {
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
        0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    };

    unsigned char const input[16] =
    {
        0x6B, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,
        0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A
    };

    unsigned char const expected[16] =
    {
        0x3A, 0xD7, 0x7B, 0xB4, 0x0D, 0x7A, 0x36, 0x60,
        0xA8, 0x9E, 0xCA, 0xF3, 0x24, 0x66, 0xEF, 0x97
    };

    unsigned char output[16];

    chat::crypto::aes::cipher aes_cipher;

    aes_cipher.set_key(crypto_key);
    aes_cipher.encrypt_block(input, output);

    BOOST_CHECK_EQUAL_COLLECTIONS(output, output + sizeof(output), expected, expected + sizeof(expected));
}

BOOST_AUTO_TEST_CASE(aes_cipher_round_trip_test)
{
    unsigned char const crypto_key[16] =
    {
        0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08,
        0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
    };

    unsigned char const input[16] =
    {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };

    unsigned char encrypted[16];
    unsigned char decrypted[16];

    chat::crypto::aes::cipher aes_cipher;

    aes_cipher.set_key(crypto_key);
    aes_cipher.encrypt_block(input, encrypted);
    aes_cipher.decrypt_block(encrypted, decrypted);

    BOOST_CHECK_EQUAL_COLLECTIONS(decrypted, decrypted + sizeof(decrypted), input, input + sizeof(input));
}

BOOST_AUTO_TEST_SUITE_END()
