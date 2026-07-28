#include "ThreadPool.hpp"

ThreadPool::ThreadPool() : m_maxThreads(std::thread::hardware_concurrency()),
    m_shouldStop(false),
    m_runningThreads(0) {
    for (unsigned int i = 0; i < m_maxThreads; i++) {
        m_threads.emplace_back([this]() {
            while (!m_shouldStop) {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_condVar.wait(lock, [this] {return !m_queue.empty() || m_shouldStop;});
                if (m_shouldStop)
                    break;

                std::function<void()> func(m_queue.front());
                m_queue.pop();
                ++m_runningThreads;
                lock.unlock();
                func();
                --m_runningThreads;
                m_waitAllCondVar.notify_all();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_shouldStop = true;
    lock.unlock();
    m_condVar.notify_all();
    for (auto& thread : m_threads) {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> func) {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_queue.emplace(func);
    lock.unlock();
    m_condVar.notify_one();
}

void ThreadPool::waitAll() {
    std::unique_lock<std::mutex> lock(m_queueMutex);
    m_waitAllCondVar.wait(lock, [this]{return !m_runningThreads && m_queue.empty();});
}
