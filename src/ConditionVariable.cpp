#include "ConditionVariable.hpp"
#include "Common.hpp"

#include <time.h>

ConditionVariable::ConditionVariable(Mutex* m)
  : m_mutex(m)
  , m_condVar(PTHREAD_COND_INITIALIZER)
{
  TRACE(this);
  pthread_cond_init( &m_condVar, 0 );
}


ConditionVariable::~ConditionVariable()
{
  TRACE(this);
  pthread_cond_destroy( &m_condVar );
}


void ConditionVariable::wait(const int interval)
{
  TRACE(this);
  if ( interval == 0 ) {
    pthread_cond_wait( &m_condVar, m_mutex->getPThreadMutex() );
  } else {
    timespec abs_time;
    clock_gettime ( CLOCK_REALTIME, &abs_time );
    abs_time.tv_nsec += interval * 1000000;
    if ( abs_time.tv_nsec >= 1000000000 ) {
      abs_time.tv_nsec -= 1000000000;
      abs_time.tv_sec += 1;
    }
    pthread_cond_timedwait( &m_condVar, m_mutex->getPThreadMutex(), &abs_time );
  }
}

void ConditionVariable::signal()
{
  TRACE(this);
  pthread_cond_signal( &m_condVar );
}

void ConditionVariable::broadcast()
{
  TRACE(this);
  pthread_cond_broadcast( &m_condVar );
}
