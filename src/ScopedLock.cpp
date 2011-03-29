#include "ScopedLock.hpp"
#include "Common.hpp"


ScopedLock::ScopedLock( Mutex& mutex) : m_mutex(mutex)
{
  TRACE;
  m_mutex.lock();
}


ScopedLock::~ScopedLock()
{
  TRACE;
  m_mutex.unlock();
}
