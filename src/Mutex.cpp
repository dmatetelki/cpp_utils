#include "Mutex.hpp"
#include "Common.hpp"


#include <time.h>


Mutex::Mutex(int kind)
{
  TRACE;
  if ( kind == PTHREAD_MUTEX_DEFAULT ) {
    pthread_mutex_init( &m_mutex, 0 );
  } else {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init( &attr );
    pthread_mutexattr_settype( &attr, kind );
    pthread_mutex_init( &m_mutex, &attr );
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


int Mutex::tryLock(const long int intervalSec)
{
  TRACE;
  if ( intervalSec == 0 ) {
    return pthread_mutex_trylock ( &m_mutex );
  } else {
    timespec tspec = addSecTotimespec( intervalSec );
    return pthread_mutex_timedlock ( &m_mutex, &tspec );
  }
}


pthread_mutex_t* Mutex::getPThreadMutex()
{
  return &m_mutex;
}
