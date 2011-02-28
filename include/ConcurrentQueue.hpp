#ifndef CONCURRENTQUEUE_HPP
#define CONCURRENTQUEUE_HPP

#include <queue>

#include "Mutex.hpp"
#include "ConditionVariable.hpp"
#include "ScopedLock.hpp"
#include "Common.hpp"



class CancelledException {};


template <typename T>
class ConcurrentQueue {

  public:

    ConcurrentQueue()
      : m_cancelled(false)
      , m_mutex()
      , m_condition(m_mutex)
    {
      TRACE(this);
    }

    ~ConcurrentQueue()
    {
      TRACE(this);
    }


    void push(const T task)
    {
      TRACE(this);
      ScopedLock sl(m_mutex);
      if (m_cancelled) throw CancelledException();
      m_queue.push( task );
      m_condition.signal();
    }


    bool tryPop(T &popped_value)
    {
      TRACE(this);
      ScopedLock sl(m_mutex);
      if (m_cancelled) throw CancelledException();
      if ( m_queue.empty() ) return false;

      popped_value = m_queue.front();
      m_queue.pop();
      return true;
    }


    T waitAndPop()
    {
      TRACE(this);
      ScopedLock sl(m_mutex);

      while ( m_queue.empty() and not m_cancelled) {
        m_condition.wait();
      }
      if (m_cancelled) throw CancelledException();

      T retVal = m_queue.front(); // cctor
      m_queue.pop();
      return retVal;
    }


    bool empty() const
    {
      TRACE(this);
      ScopedLock sl(m_mutex);
      if (m_cancelled) throw CancelledException();
      return m_queue.empty();
    }


    void cancel()
    {
      TRACE(this);
      ScopedLock sl(m_mutex);
      m_cancelled = true;
      m_condition.broadcast();
    }

  private:

    ConcurrentQueue& operator=( const ConcurrentQueue& );
    ConcurrentQueue( const ConcurrentQueue& );

    std::queue<T> m_queue;
    bool m_cancelled;
    Mutex m_mutex;
    ConditionVariable m_condition;

};

#endif // CONCURRENTQUEUE_HPP
