//---------------------------------------------------------------------------
//
//    This file is part of Chat.Informer project
//    Copyright (C) 2011, 2013, 2014 Ilya Golovenko
//
//---------------------------------------------------------------------------
#ifndef _chat_utility_concurrent_queue_hpp
#define _chat_utility_concurrent_queue_hpp

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif  // defined(_MSC_VER) && (_MSC_VER >= 1200)

// STL headers
#include <condition_variable>
#include <utility>
#include <thread>
#include <deque>


namespace chat
{

/**
 * \class concurrent_queue
 * \brief Thread-safe template queue class.
 */
template <typename T>
class concurrent_queue
{
public:
    /**
     * \brief Construct new queue.
     *
     * \param enabled Specifies whether the queue will be enabled after construction.
     */
    explicit concurrent_queue(bool enabled = true) :
        enabled_(enabled)
    {
    }

    /**
     * \brief Destroys queue.
     */
    ~concurrent_queue()
    {
        disable();
    }

    concurrent_queue(concurrent_queue const&) = delete;
    concurrent_queue& operator=(concurrent_queue const&) = delete;

    /**
     * \brief Enables queue.
     *
     * The queue will accept new items.
     */
    void enable()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        enabled_ = true;
    }

    /**
     * \brief Disables queue.
     *
     * The queue will reject new items (\a push will return \a false),
     * but remaining items can be extracted using \a pop function,
     * if \a clear parameter is false.
     *
     * \param clear Remove all items from the queue if true.
     */
    void disable(bool clear = true)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        enabled_ = false;

        if(clear)
            queue_.clear();

        lock.unlock();
        event_.notify_all();
    }

    /**
     * \brief Copies value to the queue.
     *
     * Adds new value to the queue and
     * notifies waiting consumers, if any.
     *
     * \param value Value to be added.
     * \return Returns \a true if the queue is enabled
     * and the value was added to it, \a false otherwise.
     */
    bool push(T const& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);

        if(!enabled_)
            return false;

        queue_.push_back(value);

        lock.unlock();
        event_.notify_one();

        return true;
    }

    /**
     * \brief Moves value to the queue.
     *
     * Adds new value to the queue and
     * notifies waiting consumers, if any.
     *
     * \param value Value to be added.
     * \return Returns \a true if the queue is enabled
     * and the value was added to it, \a false otherwise.
     */
    bool push(T&& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);

        if(!enabled_)
            return false;

        queue_.push_back(std::forward<T>(value));

        lock.unlock();
        event_.notify_one();

        return true;
    }

    /**
     * \brief Extracts next value from the queue.
     *
     * Tries to extracts next value from the queue.
     * If the queue is empty then this operation will block
     * until the queue will have a new value or it will be disabled.
     *
     * \param value Extracted value will be assigned to this parameter.
     * \return Returns \a true if queue is enabled and the value
     * was extracted from the queue, \a false otherwise.
     */
    bool pop(T& value)
    {
        std::unique_lock<std::mutex> lock(mutex_);

        while(queue_.empty())
        {
            if(!enabled_)
                return false;

            event_.wait(lock);
        }

        value = std::move(queue_.front());
        queue_.pop_front();

        return true;
    }

private:
    /// Indicates whether queue is enabled
    bool enabled_;

    /// Queue
    std::deque<T> queue_;

    /// Queue mutex
    std::mutex mutex_;

    /// New value event
    std::condition_variable event_;
};

}   // namespace chat

#endif  // _chat_utility_concurrent_queue_hpp
