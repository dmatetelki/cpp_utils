#include "Mutex.hpp"
#include "Common.hpp"

#include <time.h>


Mutex::Mutex(int kind)
{
  TRACE(this);
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
  TRACE(this);
  pthread_mutex_destroy ( &m_mutex );
}


int Mutex::lock()
{
  TRACE(this);
  return pthread_mutex_lock( &m_mutex );
}


int Mutex::unlock()
{
  TRACE(this);
  return pthread_mutex_unlock ( &m_mutex );
}


bool Mutex::tryLock(const int interval)
{
  TRACE(this);
  if ( interval == 0 ) {
    int result = pthread_mutex_trylock ( &m_mutex );
    return result == 0;
  } else {
    timespec abs_time;
    clock_gettime ( CLOCK_REALTIME, &abs_time );
    abs_time.tv_nsec += interval * 1000000;
    if ( abs_time.tv_nsec >= 1000000000 ) {
      abs_time.tv_nsec -= 1000000000;
      abs_time.tv_sec += 1;
    }
    int result = pthread_mutex_timedlock ( &m_mutex, &abs_time );
    return result == 0;
  }
}


pthread_mutex_t* Mutex::getPThreadMutex()
{
  return &m_mutex;
}
