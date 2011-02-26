#include "ScopedLock.hpp"
#include "Common.hpp"


ScopedLock::ScopedLock(Mutex* m) : m_mutex(m)
{
  TRACE(this);
  m_mutex->lock();
}


ScopedLock::~ScopedLock()
{
  TRACE(this);
  m_mutex->unlock();
}
