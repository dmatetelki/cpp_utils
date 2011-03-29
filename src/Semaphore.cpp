#include "Semaphore.hpp"

#include "ScopedLock.hpp"
#include "Common.hpp"


Semaphore::Semaphore( int maxCount )
  : m_maxCount( maxCount )
  , m_count( maxCount )
  , m_mutex()
  , m_condVar(m_mutex)
{
  TRACE;
}


Semaphore::~Semaphore( void )
{
  TRACE;
}


bool Semaphore::lock( const long int intervalSec )
{
  TRACE;
  ScopedLock sl(m_mutex);
  if ( m_count == 0 ) {
    if ( m_condVar.wait(intervalSec) != 0 ) {
      return false;
    }
  }
  m_count--;
  return true;
}


bool Semaphore::unLock( void )
{
  TRACE;
  ScopedLock sc(m_mutex);
  if ( m_count == m_maxCount ) {
    return false;
  }
  m_count++;
  m_condVar.signal();
  return true;
}


int Semaphore::getCount( void ) const
{
  TRACE;
  ScopedLock sc(m_mutex);
  return m_count;
}
