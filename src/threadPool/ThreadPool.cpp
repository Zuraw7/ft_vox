#include "ThreadPool.hpp"

ThreadPool::ThreadPool() : m_maxThreads(std::thread::hardware_concurrency()),
    m_shouldStop(false) {
    for (unsigned int i = 0; i < m_maxThreads; i++) {
        m_threads.emplace_back([this]() {
            while (!m_shouldStop) {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_condVar.wait(lock, [this] {return !m_queue.empty() || m_shouldStop;});
                if (m_shouldStop)
                    break;

                std::function<void()> f(m_queue.front());
                m_queue.pop();
                lock.unlock();
                f();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    m_shouldStop = true;
    m_condVar.notify_all();
    for (auto& thread : m_threads) {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> f) {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_queue.emplace(f);
    lock.unlock();
    m_condVar.notify_one();
}
