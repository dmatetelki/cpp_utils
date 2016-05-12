#ifndef CONCURRENTQUEUE_HPP
#define CONCURRENTQUEUE_HPP

#include <deque>
#include <mutex>
#include <condition_variable>

#include "Logger.hpp"


class CancelledException {};


template <typename T>
class ConcurrentDeque
{
public:

  ConcurrentDeque()
    : m_queue()
    , m_cancelled(false)
    , m_mutex()
    , m_condVar()
  {
    TRACE;
  }

  ConcurrentDeque& operator=(const ConcurrentDeque&) = delete;
  ConcurrentDeque(const ConcurrentDeque&) = delete;

  ~ConcurrentDeque()
  {
    TRACE;
  }

  void push(const T value)
  {
    TRACE;
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_cancelled) throw CancelledException();
    m_queue.push_back(value);
    m_condVar.notify_one();
  }

  T waitAndPop()
  {
    TRACE;
    std::unique_lock<std::mutex> lock(m_mutex);

    while (m_queue.empty() && !m_cancelled)
      m_condVar.wait(lock);

    if (m_cancelled) throw CancelledException();

    T retVal = m_queue.front();
    m_queue.pop_front();
    return retVal;
  }

  bool empty() const
  {
    TRACE;
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_cancelled) throw CancelledException();
    return m_queue.empty();
  }

  void cancel()
  {
    TRACE;
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cancelled = true;
    m_condVar.notify_all();
  }

private:

  std::deque<T> m_queue;
  bool m_cancelled;
  std::mutex m_mutex;
  std::condition_variable m_condVar;
};

#endif // CONCURRENTQUEUE_HPP
