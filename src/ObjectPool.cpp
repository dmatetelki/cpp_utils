#include "ObjectPool.hpp"

#include "Logger.hpp"


template <typename T>
ObjectPool<T>::ObjectPool()
  : m_pool()
{
  TRACE;
}


template <typename T>
ObjectPool<T>::~ObjectPool()
{
  TRACE;
}


template <typename T>
void ObjectPool<T>::add(const T object)
{
  TRACE;
  m_pool.push(object);
}


template <typename T>
void ObjectPool<T>::remove(const T object)
{
  TRACE;

//   m_pool.tryPop(object);
}


template <typename T>
void ObjectPool<T>::clear()
{
  TRACE;

//   while ( !m_pool.empty() )
//     m_pool.
}


template <typename T>
T ObjectPool<T>::get()
{
  TRACE;

  return m_pool.waitAndPop();
}


template <typename T>
void ObjectPool<T>::release(const T object)
{
  TRACE;

  m_pool.push(object);
}
