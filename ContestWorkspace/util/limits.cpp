#include <sys/resource.h>
#include <stdexcept>
#include <cmath>
#include <csignal>
#include <iostream>
#include <functional>
#include <memory>
#include <chrono>
#include <thread>
#include <condition_variable>

void limit_ram(float mega)
{
    if (mega > 2000)
        throw std::logic_error("Can't limit memory above 2GB");

    rlim_t bytes = static_cast<rlim_t>(mega*1024*1024);

    rlimit rlim;
    rlim.rlim_cur = bytes;
    rlim.rlim_max = bytes;
    setrlimit( RLIMIT_AS, &rlim );
}

using Duration = std::chrono::duration<float>;

struct TimeGuard {
    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    void thread_func(Duration time_limit)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.notify_all();
        auto status = m_cv.wait_for(lock, time_limit);

        if (status == std::cv_status::timeout)
        {
            std::cerr << "[CONTEST_WORKSPACE] " << "Test duration exceeded. Terminating." << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    void start(Duration time_limit)
    {
        m_thread = std::thread([=](){ thread_func(time_limit); });
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock);
    }

    ~TimeGuard()
    {
        if (m_thread.joinable())
        {
            m_cv.notify_all();
            m_thread.join();
        }
    }
};

std::shared_ptr<TimeGuard> guard_ptr;

std::shared_ptr<void> time_limit_guard()
{
    guard_ptr = std::make_shared<TimeGuard>();
    return guard_ptr;
}

void limit_cpu(float sec)
{
    guard_ptr->start(Duration(sec));
    guard_ptr.reset();
}