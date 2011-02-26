#include "Mutex.hpp"

#include <assert.h>
#include <time.h>


Mutex::Mutex(int type)
{
  int ret;
  if ( type == PTHREAD_MUTEX_DEFAULT ) {
    ret = pthread_mutex_init( &m_mutex, 0 );
  } else {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    ret = pthread_mutex_init( &m_mutex, &attr );
  }
  assert( ret == 0 );
}


Mutex::~Mutex()
{
  int ret = pthread_mutex_destroy ( &m_mutex );
  assert( ret == 0);
}


void Mutex::lock()
{
  int ret = pthread_mutex_lock( &m_mutex );
  assert( ret == 0);
}


void Mutex::unlock()
{
  int ret = pthread_mutex_unlock ( &m_mutex );
  assert( ret == 0);
}


bool Mutex::tryLock(int interval)
{
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

