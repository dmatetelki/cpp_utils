#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include "ConcurrentQueue.hpp"

template <typename T>
class ObjectPool
{
public:

  ObjectPool();
  virtual ~ObjectPool();

  void add(const T object);
  void remove(const T object);
  void clear();

  T get();
  virtual void reset(const T object) = 0;
  void release(const T object);


private:

  ConcurrentQueue<T> m_pool;

};

#endif // OBJECT_POOL_HPP
