#include "Mutex.hpp"

#include "Common.hpp"

#include <time.h>

namespace {

  pthread_mutex_t& MutexCtor( pthread_mutex_t& mutex )
  {
    pthread_mutex_init( &mutex, 0 );
    return mutex;
  }

} // anonym namespace


Mutex::Mutex( MutexType type )
  : m_mutex( MutexCtor( m_mutex ) ) // init with function
  , m_type( type )
{
  TRACE;
  if ( type != PTHREAD_MUTEX_DEFAULT ) {
    pthread_mutexattr_init( &m_attr );
    pthread_mutexattr_settype( &m_attr, type );
    pthread_mutex_init( &m_mutex, &m_attr );
  }
}


Mutex::~Mutex()
{
  TRACE;
  pthread_mutex_destroy ( &m_mutex );
}


int Mutex::lock()
{
  TRACE;
  return pthread_mutex_lock( &m_mutex );
}


int Mutex::unlock()
{
  TRACE;
  return pthread_mutex_unlock ( &m_mutex );
}


int Mutex::tryLock( const long int intervalSec, const long int intervalNSec )
{
  TRACE;
  if ( intervalSec == 0 and intervalNSec == 0 ) {
    return pthread_mutex_trylock( &m_mutex );
  } else {
    timespec tspec = addTotimespec( intervalSec, intervalSec );
    return pthread_mutex_timedlock( &m_mutex, &tspec );
  }
}