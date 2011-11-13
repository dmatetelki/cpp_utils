#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include "ConcurrentQueue.hpp"
#include "Logger.hpp"

template <typename T>
class ObjectPool
{
public:

  ObjectPool() : m_pool()
  {
    TRACE;
  }

  virtual ~ObjectPool()
  {
    TRACE;
  }


  T acquire()
  {
    TRACE;
    return m_pool.waitAndPop();
  }

  void release(const T object)
  {
    TRACE;
    m_pool.push(object);
  }

  bool empty() const
  {
    TRACE;
    return m_pool.empty();
  }


private:

  ConcurrentQueue<T> m_pool;
};

#endif // OBJECT_POOL_HPP
