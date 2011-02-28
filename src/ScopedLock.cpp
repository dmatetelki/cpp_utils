#include "ScopedLock.hpp"
#include "Common.hpp"


ScopedLock::ScopedLock( Mutex& mutex) : m_mutex(mutex)
{
  TRACE(this);
  m_mutex.lock();
}


ScopedLock::~ScopedLock()
{
  TRACE(this);
  m_mutex.unlock();
}
