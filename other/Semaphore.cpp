#include "Semaphore.hpp"

#include "ScopedLock.hpp"
#include "Common.hpp"

Semaphore::Semaphore( int maxCount )
  : m_maxCount( maxCount )
  , m_mutex()
{
  TRACE(this);
  sem_init( &m_semaphore, 0, maxCount );
}


Semaphore::~Semaphore( void )
{
  TRACE(this);
  sem_destroy( &m_semaphore );
}


int Semaphore::lock( int interval )
{
  TRACE(this);
  if ( interval == 0 ) {
    return sem_wait( &m_semaphore );
  } else {
    timespec tspec = intIntervalTotimespec( interval );
    return sem_timedwait( &m_semaphore, &tspec );
  }
}


int Semaphore::tryLock( void )
{
  TRACE(this);
  return sem_trywait( &m_semaphore );
}


int Semaphore::unLock( void )
{
  TRACE(this);
  ScopedLock sl(m_mutex); // is it needed?
  int value;
  sem_getvalue( &m_semaphore, &value);
  if ( value == m_maxCount ) return -1;
  return sem_post( &m_semaphore );
}


int Semaphore::getCount( void ) const
{
  TRACE(this);
  int retval;
  sem_getvalue( &m_semaphore, &retval );
  return retval;
}
