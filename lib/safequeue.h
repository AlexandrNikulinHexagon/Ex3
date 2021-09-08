#pragma once

#include <queue>
#include <mutex>

template <class T>
class SafeQueue
{
public:
    class Stop {};

    void push(const T& t)
    {
        std::lock_guard lock(mutex);
        m_queue.push(t);
        queue_non_empty.notify_one();
    }

    auto pop(void)
    {
        std::unique_lock lock(mutex);

        queue_non_empty.wait(lock, [this]() {return !m_queue.empty() || m_stop; });
        if (m_stop && m_queue.empty())
            throw Stop();

        auto val = m_queue.front();
        m_queue.pop();

        if (m_stop && m_queue.empty())
            queue_empty.notify_all();

        return val;
    }

    void wait_empty()
    {
        std::unique_lock lock(mutex);
        m_stop = true;
        queue_empty.wait(lock, [this]() { return m_queue.empty(); });
        queue_non_empty.notify_all();
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex mutex;
    std::condition_variable queue_non_empty, queue_empty;
    bool m_stop = false;
};