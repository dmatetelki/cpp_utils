#include "Thread.hpp"
#include "Common.hpp"

#include <signal.h>
#include <iostream>

Thread::Thread() : m_nThread( 0 )
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
  pthread_create( &m_nThread, NULL, threadStarter, ( void* )this );
}


void* Thread::join() const
{
  TRACE(this);
  void* retVal;
  pthread_join( m_nThread, &retVal );
  return retVal;
}


void Thread::sendSignal( const int nSignal ) const
{
  TRACE(this);
  pthread_kill( m_nThread, nSignal );
}


void* Thread::threadStarter( void* pData )
{
  TRACE("Thread::threadStarter");
  return static_cast<Thread *>(pData)->run();
}
