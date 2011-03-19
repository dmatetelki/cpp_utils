#include "ConditionVariable.hpp"
#include "Common.hpp"

#include <time.h>


ConditionVariable::ConditionVariable(Mutex& mutex)
  : m_mutex(mutex)
{
  TRACE(this);
  pthread_cond_init( &m_condVar, 0 );
}


ConditionVariable::~ConditionVariable()
{
  TRACE(this);
  pthread_cond_destroy( &m_condVar );
}


int ConditionVariable::wait(const int interval)
{
  TRACE(this);
  if ( interval == 0 ) {
    return pthread_cond_wait( &m_condVar,
                                 m_mutex.getPThreadMutex() );
  } else {
    timespec tspec = intIntervalTotimespec(interval);
    return pthread_cond_timedwait( &m_condVar,
                                    m_mutex.getPThreadMutex(),
                                   &tspec);
  }
}

int ConditionVariable::signal()
{
  TRACE(this);
  return pthread_cond_signal( &m_condVar );
}

int ConditionVariable::broadcast()
{
  TRACE(this);
  return pthread_cond_broadcast( &m_condVar );
}
