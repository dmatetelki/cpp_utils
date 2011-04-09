#include "Thread.hpp"
#include "Common.hpp"

#include <signal.h>
#include <iostream>
#include <exception> // std::runtime_error

#include <sched.h> // sched_param


Thread::Thread()
  : m_isRunning(false)
  , m_threadHandler( 0 )
{
  TRACE;
}


Thread::~Thread()
{
  TRACE;
}


void Thread::start()
{
  TRACE;
  m_isRunning = true;
  pthread_create( &m_threadHandler, 0, threadStarter, ( void* )this );
}


void* Thread::join() const
{
  TRACE;
  void* retVal;
  pthread_join( m_threadHandler, &retVal );
  return retVal;
}


void Thread::stop()
{
  TRACE;
  m_isRunning = false;
}


void Thread::sendSignal( const int nSignal ) const
{
  TRACE;
  pthread_kill( m_threadHandler, nSignal );
}


void* Thread::threadStarter( void* pData )
{
  TRACE_STATIC;
  return static_cast<Thread *>(pData)->run();
}
