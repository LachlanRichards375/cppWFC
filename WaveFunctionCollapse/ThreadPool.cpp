#include "ThreadPool.h"

//https://stackoverflow.com/questions/15752659/thread-pooling-in-c11
void ThreadPool::Start(uint32_t MaxThreadCount) {
    const uint32_t num_threads = MaxThreadCount; // Max # of threads the system supports
    for (uint32_t ii = 0; ii < num_threads; ++ii) {
        threads.emplace_back(std::thread(&ThreadPool::ThreadLoop, this));
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        ZoneScopedN("ThreadLoop");
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            {
                ZoneScopedN("Waiting For Lock");
                mutex_condition.wait(lock, [this] {
                    return !jobs.empty() || should_terminate;
                    });
            }
            {
                ZoneScopedN("Lock Aquired");
                if (should_terminate) {
                    return;
                }
                job = jobs.front();
                jobs.pop();
            }
        }
        {
            ZoneScopedN("Performing Job");
            job();
        }
    }
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = !jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::Stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}