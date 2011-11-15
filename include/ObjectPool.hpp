#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include "ConcurrentDeque.hpp"
#include "Logger.hpp"

template <typename T>
class ObjectPool
{
public:

  ObjectPool()
    : m_pool()
    , m_numberOfUsedObjects(0)
  {
    TRACE;
  }

  virtual ~ObjectPool()
  {
    TRACE;
  }

  void add(const T object) // throws CancelledException
  {
    TRACE;
    m_pool.push(object);
  }


  T acquire() // throws CancelledException
  {
    TRACE;
    T tmp = m_pool.waitAndPop();
    m_numberOfUsedObjects++;
    return tmp;
  }

  template<class U = T>
  typename std::enable_if< std::is_pointer<U>::value >::type
  release(T object)
  {
    TRACE;
    if ( m_pool.cancelled() ) {
      m_numberOfUsedObjects--;
      delete object;
      return;
    }

    m_pool.push(object);
    m_numberOfUsedObjects--;
  }

  template<class U = T>
  typename std::enable_if< !(std::is_pointer<U>::value) >::type
  release(T object)
  {
    TRACE;
    if ( m_pool.cancelled() ) {
      m_numberOfUsedObjects--;
      return;
    }

    m_pool.push(object);
    m_numberOfUsedObjects--;
  }

  void clear()
  {
    TRACE;
    m_pool.cancel();
  }


private:

  ConcurrentDeque<T> m_pool;
  int m_numberOfUsedObjects;
};

#endif // OBJECT_POOL_HPP
