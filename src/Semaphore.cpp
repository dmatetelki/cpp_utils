#include "Semaphore.hpp"

#include "ScopedLock.hpp"
#include "Common.hpp"

Semaphore::Semaphore( int maxCount )
  : m_maxCount( maxCount )
  , m_count( maxCount )
  , m_mutex()
  , m_condVar(m_mutex)
{
  TRACE(this);
}


Semaphore::~Semaphore( void )
{
  TRACE(this);
}


bool Semaphore::lock( int interval )
{
  TRACE(this);
  ScopedLock sl(m_mutex);
  if ( m_count == 0 ) {
    if ( m_condVar.wait(interval) != 0 ) {
      return false;
    }
  }
  m_count -= 1;
  return true;
}


bool Semaphore::unLock( void )
{
  TRACE(this);
  ScopedLock sc(m_mutex);
  if ( m_count == m_maxCount ) {
    return false;
  }
  m_count += 1;
  m_condVar.signal();
  return true;
}


int Semaphore::getCount( void ) const
{
  TRACE(this);
  ScopedLock sc(m_mutex);
  return m_count;
}
