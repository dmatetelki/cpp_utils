#ifndef CONCURRENTMULTIMAP_HPP
#define CONCURRENTMULTIMAP_HPP

#include <map>
#include <vector>

#include "Mutex.hpp"
#include "ConditionVariable.hpp"
#include "ScopedLock.hpp"
#include "Common.hpp"


template < typename T1, typename T2 >
class ConcurrentMultiMap
{
public:

  class CancelledException {};

  ConcurrentMultiMap()
    : m_mutex()
    , m_condVar(m_mutex)
    , m_cancelled(false)
    , m_map()
  {
    TRACE;
  }

  ~ConcurrentMultiMap()
  {
    TRACE;
  }

  typename std::multimap<T1, T2>::iterator begin()
  {
    return m_map.begin();
  }

  typename std::multimap<T1, T2>::const_iterator begin() const
  {
    return m_map.begin();
  }

  typename std::multimap<T1, T2>::iterator end()
  {
    return m_map.end();
  }

  typename std::multimap<T1, T2>::const_iterator end() const
  {
    return m_map.end();
  }


  void add(T1 key, T2 value)
  {
    TRACE;
    ScopedLock sl(m_mutex);
    if (m_cancelled) throw CancelledException();
    m_map.insert( std::make_pair<T1, T1>( key, value) );
    m_condVar.signal();
  }

  bool erase( T1 key )
  {
    TRACE;
    ScopedLock sl( m_mutex );
    if ( m_map.erase(key) > 0 ) {
      m_condVar.signal();
      return true;
    }
    return false;
  }

  bool removeValue( T2 value )
  {
    TRACE;
    ScopedLock sl(m_mutex);
    typename std::multimap<T1, T2>::iterator it;
    for ( it = m_map.begin(); it != m_map.end(); it++ ) {
      if (it->second == value) {
        m_map.erase(it);
        m_condVar.signal();
        return true;
      }
    }
    return false;
  }

  typename std::pair< std::multimap<T1, T2>::iterator, std::multimap<T1, T2>::iterator >  equal_range ( const T1& key )
  {
    TRACE;
    ScopedLock sl( m_mutex );
    return m_map.equal_range(key);
  }

  T1 waitAndGetFirstKey()
  {
    TRACE;
    ScopedLock sl(m_mutex);

    while ( m_map.empty() and not m_cancelled) {
      m_condVar.wait();
    }
    if (m_cancelled) throw CancelledException();

    return m_map.begin()->first;
  }

  bool empty() const
  {
    TRACE;
    ScopedLock sl(m_mutex);
    if (m_cancelled) throw CancelledException();
    return m_map.empty();
  }

  void cancel( void )
  {
    TRACE;
    ScopedLock sl(m_mutex);
    m_cancelled = true;
    m_condVar.broadcast();
  }

private:

  ConcurrentMultiMap& operator=( const ConcurrentMultiMap& );
  ConcurrentMultiMap( const ConcurrentMultiMap& );

  mutable Mutex m_mutex;
  ConditionVariable m_condVar;
  bool m_cancelled;
  std::multimap<T1, T2> m_map;

};

#endif // CONCURRENTMULTIMAP_HPP
