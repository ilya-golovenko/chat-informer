//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//--------------------------------------------------------------------------- 

// Application headers
#include <utility/raw_buffer.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>

// STL headers
#include <sstream>

BOOST_AUTO_TEST_SUITE(raw_buffer_test_suite)

struct raw_buffer_fixture
{
    static const std::size_t value_size = 4;

    raw_buffer_fixture() :
        buffer_with_value(value_size)
    {
        std::memcpy(buffer_value, "1234", value_size);
        std::memcpy(buffer_with_value.data(), "1234", value_size);
    }

    unsigned char buffer_value[value_size];
    chat::raw_buffer buffer_with_value;
};

BOOST_FIXTURE_TEST_CASE(constructor_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer1;
    chat::raw_buffer buffer2(10);
    chat::raw_buffer buffer3(1000);
    chat::raw_buffer buffer4(buffer_value, value_size);

    BOOST_CHECK(buffer1.empty());
    BOOST_CHECK(buffer1.data() == nullptr);
    BOOST_CHECK_EQUAL(buffer1.size(), 0u);

    BOOST_CHECK(!buffer2.empty());
    BOOST_CHECK(buffer2.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer2.size(), 10u);

    BOOST_CHECK(!buffer3.empty());
    BOOST_CHECK(buffer3.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer3.size(), 1000u);

    BOOST_CHECK(!buffer4.empty());
    BOOST_CHECK(buffer4.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer4, buffer_with_value);
}

BOOST_FIXTURE_TEST_CASE(copy_constructor_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer(buffer_with_value);

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer, buffer_with_value);
}

BOOST_FIXTURE_TEST_CASE(assignment_operator_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer;
    buffer = buffer_with_value;

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer, buffer_with_value);

    buffer = buffer;    // self-assignment

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer, buffer_with_value);
}

BOOST_FIXTURE_TEST_CASE(assign_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer1;
    chat::raw_buffer buffer2;

    buffer1.assign(buffer_with_value);

    BOOST_CHECK(!buffer1.empty());
    BOOST_CHECK(buffer1.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer1, buffer_with_value);

    buffer2.assign(buffer_value, value_size);

    BOOST_CHECK(!buffer2.empty());
    BOOST_CHECK(buffer2.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer2, buffer_with_value);

    buffer2.assign(buffer2);    // self-assignment

    BOOST_CHECK(!buffer2.empty());
    BOOST_CHECK(buffer2.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer2, buffer_with_value);
}

BOOST_FIXTURE_TEST_CASE(append_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer1(buffer_with_value);
    chat::raw_buffer buffer2(buffer_with_value);

    buffer1.append(buffer_with_value);

    BOOST_CHECK(!buffer1.empty());
    BOOST_CHECK(buffer1.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer1.size(), value_size * 2u);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer1.data(), buffer1.data() + value_size,
        buffer_with_value.data(), buffer_with_value.data() + value_size);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        buffer1.data() + value_size, buffer1.data() + value_size * 2u,
        buffer_with_value.data(), buffer_with_value.data() + value_size);

    buffer2.append(buffer_value, value_size);

    BOOST_CHECK(!buffer2.empty());
    BOOST_CHECK(buffer2.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer2.size(), value_size * 2u);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer2.data(), buffer2.data() + value_size,
        buffer_with_value.data(), buffer_with_value.data() + value_size);

    BOOST_CHECK_EQUAL_COLLECTIONS(
        buffer2.data() + value_size, buffer2.data() + value_size * 2u,
        buffer_with_value.data(), buffer_with_value.data() + value_size);
}

BOOST_FIXTURE_TEST_CASE(clear_test, raw_buffer_fixture)
{
    buffer_with_value.clear();

    BOOST_CHECK(buffer_with_value.empty());
    BOOST_CHECK(buffer_with_value.data() == nullptr);
    BOOST_CHECK_EQUAL(buffer_with_value.size(), 0u);
}

BOOST_FIXTURE_TEST_CASE(grow_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer(buffer_with_value);

    buffer.grow(32);

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), 32u + value_size);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer.data(), buffer.data() + value_size,
        buffer_with_value.data(), buffer_with_value.data() + value_size);

    buffer.grow(0);

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), 32u + value_size);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer.data(), buffer.data() + value_size,
        buffer_with_value.data(), buffer_with_value.data() + value_size);
}

BOOST_FIXTURE_TEST_CASE(shrink_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer(buffer_with_value);

    buffer.shrink(value_size - 2);

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), value_size - 2u);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer.data(), buffer.data() + value_size - 2,
        buffer_with_value.data(), buffer_with_value.data() + value_size - 2);

    buffer.shrink(32);

    BOOST_CHECK(buffer.empty());
    BOOST_CHECK(buffer.data() == nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), 0u);
}

BOOST_FIXTURE_TEST_CASE(resize_test, raw_buffer_fixture)
{
    chat::raw_buffer buffer(buffer_with_value);

    buffer.resize(128);

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), 128u);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer.data(), buffer.data() + value_size,
        buffer_with_value.data(), buffer_with_value.data() + value_size);

    buffer.resize(32);

    BOOST_CHECK(!buffer.empty());
    BOOST_CHECK(buffer.data() != nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), 32u);

    BOOST_CHECK_EQUAL_COLLECTIONS(buffer.data(), buffer.data() + value_size,
        buffer_with_value.data(), buffer_with_value.data() + value_size);

    buffer.resize(0);

    BOOST_CHECK(buffer.empty());
    BOOST_CHECK(buffer.data() == nullptr);
    BOOST_CHECK_EQUAL(buffer.size(), 0u);
}

BOOST_AUTO_TEST_CASE(capacity_test)
{
    chat::raw_buffer buffer1;
    chat::raw_buffer buffer2(10);
    chat::raw_buffer buffer3(120);
    chat::raw_buffer buffer4(1024);

    BOOST_CHECK_EQUAL(buffer1.capacity(), 64u);
    BOOST_CHECK_EQUAL(buffer2.capacity(), 64u);
    BOOST_CHECK_EQUAL(buffer3.capacity(), 128u);
    BOOST_CHECK_EQUAL(buffer4.capacity(), 1024u);
}

BOOST_AUTO_TEST_CASE(index_operator_test)
{
    chat::raw_buffer buffer(16);
    std::memset(buffer.data(), 0x00, buffer.size());

    BOOST_CHECK_EQUAL(buffer[0], 0x00u);
    BOOST_CHECK_EQUAL(buffer[10], 0x00u);
    BOOST_CHECK_EQUAL(buffer[buffer.size() - 1], 0x00u);

    buffer[10] = 0xAB;
    buffer[buffer.size() - 1] = 0xCD;

    BOOST_CHECK_EQUAL(buffer[10], 0xABu);
    BOOST_CHECK_EQUAL(*(buffer.data() + 10), 0xABu);
    BOOST_CHECK_EQUAL(buffer[buffer.size() - 1], 0xCDu);
}

BOOST_AUTO_TEST_CASE(stream_printable_buffer_operator_test)
{
    chat::raw_buffer buffer("Hello, world!", std::strlen("Hello, world!"));

    std::ostringstream stream;
    stream << buffer;

    BOOST_CHECK_EQUAL(stream.str(), "Hello, world!");
}

BOOST_AUTO_TEST_CASE(stream_non_printable_buffer_operator_test)
{
    chat::raw_buffer buffer("\x0A\x0B\x0C", std::strlen("\x0A\x0B\x0C"));

    std::ostringstream stream;
    stream << buffer;

    BOOST_CHECK_EQUAL(stream.str(), "\\x0A\\x0B\\x0C");
}

BOOST_AUTO_TEST_CASE(stream_mixed_buffer_operator_test)
{
    chat::raw_buffer buffer("ABC\x0A\x0B\x0C", std::strlen("ABC\x0A\x0B\x0C"));

    std::ostringstream stream;
    stream << buffer;

    BOOST_CHECK_EQUAL(stream.str(), "ABC\\x0A\\x0B\\x0C");
}

BOOST_FIXTURE_TEST_CASE(compaison_operators_test, raw_buffer_fixture)
{
    chat::raw_buffer same_buffer(buffer_with_value);
    chat::raw_buffer other_buffer("ABCD", std::strlen("ABCD"));

    BOOST_CHECK_EQUAL(buffer_with_value, same_buffer);
    BOOST_CHECK_NE(buffer_with_value, other_buffer);
    BOOST_CHECK_NE(same_buffer, other_buffer);
    BOOST_CHECK_LT(same_buffer, other_buffer);
}

BOOST_AUTO_TEST_SUITE_END()
