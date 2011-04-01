#include "ConditionVariable.hpp"
#include "Common.hpp"

#include <time.h>


ConditionVariable::ConditionVariable(Mutex& mutex)
  : m_mutex(mutex)
{
  TRACE;
  pthread_cond_init( &m_condVar, 0 );
}


ConditionVariable::~ConditionVariable()
{
  TRACE;
  pthread_cond_destroy( &m_condVar );
}


int ConditionVariable::wait(const long int intervalSec, const long int intervalNSec)
{
  TRACE;
  if ( intervalSec == 0 and intervalNSec == 0 ) {
    return pthread_cond_wait( &m_condVar,
                               m_mutex.getPThreadMutex() );
  } else {
    timespec tspec = addTotimespec( intervalSec, intervalNSec );
    return pthread_cond_timedwait( &m_condVar,
                                    m_mutex.getPThreadMutex(),
                                   &tspec);
  }
}


int ConditionVariable::signal()
{
  TRACE;
  return pthread_cond_signal( &m_condVar );
}


int ConditionVariable::broadcast()
{
  TRACE;
  return pthread_cond_broadcast( &m_condVar );
}
