#ifndef CONCURRENTQUEUE_HPP
#define CONCURRENTQUEUE_HPP

#include <deque>
#include <algorithm>
#include <type_traits>

#include "Mutex.hpp"
#include "ConditionVariable.hpp"
#include "ScopedLock.hpp"
#include "Common.hpp"



class CancelledException {};


template <typename T>
class ConcurrentDeque
{
public:

  ConcurrentDeque()
    : m_queue()
    , m_cancelled(false)
    , m_mutex()
    , m_condVar(m_mutex)
  {
    TRACE;
  }

  ~ConcurrentDeque()
  {
    TRACE;
    freeDeque();
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

  void cancel()
  {
    TRACE;
    ScopedLock sl(m_mutex);
    m_cancelled = true;
    m_condVar.broadcast();
    freeDeque();
  }

  template<class U = T>
  typename std::enable_if< std::is_pointer<U>::value >::type
  freeDeque()
  {
    TRACE;
    typename std::deque<T>::iterator it;
    for ( it = m_queue.begin(); it != m_queue.end(); ++it )
      delete *it;

    m_queue.clear();
  }

  template<class U = T>
  typename std::enable_if< !(std::is_pointer<U>::value) >::type
  freeDeque()
  {
    TRACE;
    m_queue.clear();
  }

  bool cancelled()
  {
    TRACE;
    return m_cancelled;
  }

private:


  ConcurrentDeque& operator=( const ConcurrentDeque& );
  ConcurrentDeque( const ConcurrentDeque& );

  std::deque<T> m_queue;
  bool m_cancelled;
  mutable Mutex m_mutex;
  ConditionVariable m_condVar;

};

#endif // CONCURRENTQUEUE_HPP
