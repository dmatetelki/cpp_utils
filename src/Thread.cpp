#include "Thread.hpp"
#include "Common.hpp"

#include <signal.h>
#include <iostream>

Thread::Thread()
  : m_isRunning(false)
  , m_threadHandler( 0 )

{
  TRACE(this);
}


Thread::~Thread()
{
  TRACE(this);
}


void Thread::start()
{
  TRACE(this);
  m_isRunning = true;
  pthread_create( &m_threadHandler, NULL, threadStarter, ( void* )this );
}


void* Thread::join() const
{
  TRACE(this);
  void* retVal;
  pthread_join( m_threadHandler, &retVal );
  return retVal;
}


void Thread::stop()
{
  TRACE(this);
  m_isRunning = false;
}


void Thread::sendSignal( const int nSignal ) const
{
  TRACE(this);
  pthread_kill( m_threadHandler, nSignal );
}


void* Thread::threadStarter( void* pData )
{
  TRACE("Thread::threadStarter");
  return static_cast<Thread *>(pData)->run();
}
