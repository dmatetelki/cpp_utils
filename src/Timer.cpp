#include "Timer.hpp"

#include "Common.hpp"

#include <signal.h> // sigset_t


Timer::Timer( const int signal )
    : m_signal( signal )
    , m_timerId( 0 )
    , m_periodic( false )
    , m_running( true )
{
  TRACE;
}

Timer::~Timer()
{
  TRACE;

  struct itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  timer_settime( m_timerId, 0, &its, 0 );
}


void Timer::createTimer( const time_t interval_sec,
                         const long interval_nsec,
                         const time_t initExpr_sec,
                         const long initExpr_nsec )
{
  TRACE;

  // create timer
  struct sigevent sigev;
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = m_signal;
  sigev.sigev_value.sival_ptr = &m_timerId;
  timer_create( CLOCK_REALTIME, &sigev, &m_timerId );

  LOG( Logger::FINEST, ( std::string( "Timer created with ID: " ) +
                         stringify( m_timerId ) ).c_str() );

  // arm it
  struct itimerspec its;
  its.it_value.tv_sec = interval_sec;
  its.it_value.tv_nsec = interval_nsec;
  its.it_interval.tv_sec = initExpr_sec;
  its.it_interval.tv_nsec = initExpr_nsec;

  if ( initExpr_sec != 0 or initExpr_nsec != 0 ) m_periodic = true;
  timer_settime( m_timerId, 0, &its, 0 );
}


void Timer::wait()
{
  TRACE;

  /// @note timerID is acquired from the siginfo after all
  long* tidp;
  siginfo_t sigInfo;

  sigset_t sigSet;
  sigemptyset( &sigSet );
  sigaddset( &sigSet, m_signal );

  sigwaitinfo( &sigSet, &sigInfo);
  if ( not m_running ) return;
  tidp = (long*)sigInfo.si_value.sival_ptr;

//   LOG( Logger::FINEST, ( std::string( "got signal: " ) +
//                          stringify( sigInfo.si_signo ) +
//                          " from: " + stringify( (timer_t)*tidp ) ).c_str() );

  timerExpired();

  if ( m_periodic ) {
    while ( m_running ) {

      sigwaitinfo( &sigSet, &sigInfo);
      if ( not m_running ) return;
      tidp = (long*)sigInfo.si_value.sival_ptr;

//   LOG( Logger::FINEST, ( std::string( "got periodic signal: " ) +
//                          stringify( sigInfo.si_signo ) +
//                          " from: " + stringify( (timer_t)*tidp ) ).c_str() );

      periodicTimerExpired();
    }
  }

}


void Timer::stopTimer()
{
  TRACE;

  // disarm timer
  struct itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  timer_settime( m_timerId, 0, &its, 0 );

  m_running = false;
  /// @note sigwaitinfo waiting state, don't forget to send a last signal
}


void Timer::gracefulStop()
{
  TRACE;

  // if it's periodic, use stopTimer
  m_running = false;
  /// @note sigwaitinfo waiting state, don't forget to send a last signal
}
