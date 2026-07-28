#ifndef FT_VOX_THREADPOOL_HPP
#define FT_VOX_THREADPOOL_HPP
#include <atomic>
#include <condition_variable>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool &other) = delete;
    ThreadPool(ThreadPool &&other) noexcept = delete;
    ThreadPool &operator=(const ThreadPool &other) = delete;
    ThreadPool &operator=(ThreadPool &&other) noexcept = delete;

    void enqueue(std::function<void()> func);
    void waitAll();

private:
    unsigned int m_maxThreads;
    std::atomic<bool> m_shouldStop;
    std::atomic<int> m_runningThreads;
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_queue;
    std::mutex m_queueMutex;
    std::condition_variable m_condVar;
    std::condition_variable m_waitAllCondVar;
};


#endif //FT_VOX_THREADPOOL_HPP