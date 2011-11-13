#ifndef CONCURRENTQUEUE_HPP
#define CONCURRENTQUEUE_HPP

#include <deque>
#include <algorithm>

#include "Mutex.hpp"
#include "ConditionVariable.hpp"
#include "ScopedLock.hpp"
#include "Common.hpp"



class CancelledException {};


template <typename T>
class ConcurrentQueue
{
public:

  ConcurrentQueue()
    : m_queue()
    , m_cancelled(false)
    , m_mutex()
    , m_condVar(m_mutex)
  {
    TRACE;
  }

  ~ConcurrentQueue()
  {
    TRACE;
  }


  void push(const T value)
  {
    TRACE;
    ScopedLock sl(m_mutex);
    if (m_cancelled) throw CancelledException();
    m_queue.push_back( value );
    m_condVar.signal();
  }


  bool tryPop(T &popped_value)
  {
    TRACE;
    ScopedLock sl(m_mutex);
    if (m_cancelled) throw CancelledException();
    if ( m_queue.empty() ) return false;

    popped_value = m_queue.front();
    m_queue.pop_front();
    return true;
  }


  T waitAndPop()
  {
    TRACE;
    ScopedLock sl(m_mutex);

    while ( m_queue.empty() and not m_cancelled) {
      m_condVar.wait();
    }
    if (m_cancelled) throw CancelledException();

    T retVal = m_queue.front(); // cctor
    m_queue.pop_front();
    return retVal;
  }


  bool empty() const
  {
    TRACE;
    ScopedLock sl(m_mutex);
    if (m_cancelled) throw CancelledException();
    return m_queue.empty();
  }


  void cancel(T a)
  {
    TRACE;
    ScopedLock sl(m_mutex);
    m_cancelled = true;
    m_condVar.broadcast();

    m_queue.clear();
  }

    void cancel(T *a)
  {
    TRACE;
    ScopedLock sl(m_mutex);
    m_cancelled = true;
    m_condVar.broadcast();

    typename std::deque<T*>::iterator it;
    for ( it = m_queue.begin(); it != m_queue.end(); ++it ) {
      LOG( Logger::INFO, std::string("Deleting: ").append(*it).c_str() );
      delete *it;
    }
    m_queue.clear();
  }

  bool cancelled()
  {
    TRACE;
    return m_cancelled;
  }

private:


  ConcurrentQueue& operator=( const ConcurrentQueue& );
  ConcurrentQueue( const ConcurrentQueue& );

  std::deque<T> m_queue;
  bool m_cancelled;
  mutable Mutex m_mutex;
  ConditionVariable m_condVar;

};

#endif // CONCURRENTQUEUE_HPP
