#include "ConditionVariable.hpp"
#include "Common.hpp"

#include <time.h>
#include <assert.h>

ConditionVariable::ConditionVariable(Mutex& mutex)
  : m_mutex(mutex)
{
  TRACE(this);
  int ret = pthread_cond_init( &m_condVar, 0 );
  assert( ret == 0);
}


ConditionVariable::~ConditionVariable()
{
  TRACE(this);
  int ret = pthread_cond_destroy( &m_condVar );
  assert( ret == 0);
}


void ConditionVariable::wait(const int interval)
{
  TRACE(this);
  if ( interval == 0 ) {
    int ret = pthread_cond_wait( &m_condVar,
                                 m_mutex.getPThreadMutex() );
    assert( ret == 0);
  } else {
    timespec abs_time;
    clock_gettime ( CLOCK_REALTIME, &abs_time );
    abs_time.tv_nsec += interval * 1000000;
    if ( abs_time.tv_nsec >= 1000000000 ) {
      abs_time.tv_nsec -= 1000000000;
      abs_time.tv_sec += 1;
    }
    int ret = pthread_cond_timedwait( &m_condVar,
                                      m_mutex.getPThreadMutex(),
                                      &abs_time );
    assert( ret == 0);
  }
}

void ConditionVariable::signal()
{
  TRACE(this);
  int ret = pthread_cond_signal( &m_condVar );
  assert( ret == 0);
}

void ConditionVariable::broadcast()
{
  TRACE(this);
  int ret = pthread_cond_broadcast( &m_condVar );
  assert( ret == 0);
}
