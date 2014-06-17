//---------------------------------------------------------------------------
//
//    This file is part of Chat Informer project
//    Copyright (C) 2011, 2013 Ilya Golovenko
//
//--------------------------------------------------------------------------- 

// Application headers
#include <utility/concurrent_queue.hpp>

// BOOST headers
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>


BOOST_AUTO_TEST_SUITE(concurrent_queue_test_suite)

struct queue_pop_fixture
{
    queue_pop_fixture() :
        sum(0),
        barrier(4)
    {
    }

    void pop_value()
    {
        barrier.wait();

        int local_sum = 0;
        int local_value;

        while(queue.pop(local_value))
            local_sum += local_value;

        boost::mutex::scoped_lock lock(sum_mutex);
        sum += local_sum;
    }

    int sum;
    boost::barrier barrier;
    boost::mutex sum_mutex;
    boost::thread_group threads;
    util::concurrent_queue<int> queue;
};

struct queue_disable_fixture
{
    queue_disable_fixture() :
        hello_string("Hello, "),
        world_string("world!")
    {
        queue.push(hello_string);
        queue.push(world_string);
    }

    std::string const hello_string;
    std::string const world_string;
    util::concurrent_queue<std::string> queue;
};

struct consumer_producer_fixture
{
    consumer_producer_fixture() :
        sum(0),
        barrier(4)
    {
    }

    void produce()
    {
        barrier.wait();

        for(int i = 0; i < 5000; ++i)
            queue.push(i);
    }

    void consume()
    {
        barrier.wait();

        int local_sum = 0;
        int local_value;

        while(queue.pop(local_value))
            local_sum += local_value;

        boost::mutex::scoped_lock lock(sum_mutex);
        sum += local_sum;
    }

    int sum;
    boost::barrier barrier;
    boost::mutex sum_mutex;
    boost::thread_group producer_threads;
    boost::thread_group consumer_threads;
    util::concurrent_queue<int> queue;
};

BOOST_FIXTURE_TEST_CASE(concurrent_queue_pop_test, queue_pop_fixture)
{
    for(int i = 0; i < 10000; ++i)
        BOOST_CHECK(queue.push(i));

    int result;

    BOOST_CHECK(queue.pop(result));
    BOOST_CHECK_EQUAL(result, 0);

    for(int i = 0; i < 4; ++i)
    {
        threads.create_thread(boost::bind(
            &queue_pop_fixture::pop_value, this));
    }

    queue.disable(false);
    threads.join_all();

    BOOST_CHECK(!queue.pop(result));
    BOOST_CHECK_EQUAL(sum, 49995000);
}

BOOST_FIXTURE_TEST_CASE(concurrent_queue_disable_test, queue_disable_fixture)
{
    std::string result;

    BOOST_CHECK(queue.pop(result));
    BOOST_CHECK_EQUAL(result, hello_string);

    queue.disable(false);

    BOOST_CHECK(!queue.push("value"));

    BOOST_CHECK(queue.pop(result));
    BOOST_CHECK_EQUAL(result, world_string);

    BOOST_CHECK(!queue.pop(result));
}

BOOST_FIXTURE_TEST_CASE(concurrent_queue_disable_clear_test, queue_disable_fixture)
{
    std::string result;

    BOOST_CHECK(queue.pop(result));
    BOOST_CHECK_EQUAL(result, hello_string);

    queue.disable(true);

    BOOST_CHECK(!queue.push("value"));
    BOOST_CHECK(!queue.pop(result));
}

BOOST_FIXTURE_TEST_CASE(concurrent_queue_enable_test, queue_disable_fixture)
{
    std::string result;

    BOOST_CHECK(queue.pop(result));
    BOOST_CHECK_EQUAL(result, hello_string);

    queue.disable(false);

    BOOST_CHECK(!queue.push("value"));

    queue.enable();

    BOOST_CHECK(queue.pop(result));
    BOOST_CHECK_EQUAL(result, world_string);

    BOOST_CHECK(queue.push("value"));
}

BOOST_FIXTURE_TEST_CASE(consumer_producer_test, consumer_producer_fixture)
{
    for(int i = 0; i < 2; ++i)
    {
        producer_threads.create_thread(boost::bind(
            &consumer_producer_fixture::produce, this));

        consumer_threads.create_thread(boost::bind(
            &consumer_producer_fixture::consume, this));
    }

    producer_threads.join_all();

    queue.disable(false);
    consumer_threads.join_all();

    int result;

    BOOST_CHECK(!queue.pop(result));
    BOOST_CHECK_EQUAL(sum, 24995000);
}

BOOST_AUTO_TEST_SUITE_END()
