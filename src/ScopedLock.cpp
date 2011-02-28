#include "ScopedLock.hpp"
#include "Common.hpp"


ScopedLock::ScopedLock( const Mutex* const mutex) : m_mutex(mutex)
{
  TRACE(this);
  const_cast<Mutex*>(m_mutex)->lock();
}


ScopedLock::~ScopedLock()
{
  TRACE(this);
  const_cast<Mutex*>(m_mutex)->unlock();
}
