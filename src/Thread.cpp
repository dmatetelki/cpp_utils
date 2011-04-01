#include "Thread.hpp"
#include "Common.hpp"

#include <signal.h>
#include <iostream>
#include <exception> // std::runtime_error

#include <sched.h> // sched_param


Thread::Thread(const bool softRealTime)
  : m_isRunning(false)
  , m_threadHandler( 0 )
  , m_softRealTime(softRealTime)
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

  if ( m_softRealTime ) {
    pthread_attr_t attr;
    sched_param param;

    pthread_attr_init(&attr);
    if ( pthread_attr_setschedpolicy( &attr, SCHED_RR ) != 0 ) {
      throw std::runtime_error( "Coudn't set thread scheduler." );
    }
    param.sched_priority = 50;
    if ( pthread_attr_setschedparam( &attr, &param ) != 0 ) {
      throw std::runtime_error( "Coudn't set thread priority.");
    }

    pthread_create( &m_threadHandler, &attr, threadStarter, ( void* )this );
  } else {
    pthread_create( &m_threadHandler, 0, threadStarter, ( void* )this );
  }
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
  LOG(Logger::FINEST, "static");
  return static_cast<Thread *>(pData)->run();
}
