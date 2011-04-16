#include "Timer.hpp"

#include "Common.hpp"

#include <signal.h> // sigset_t
#include <time.h> // timer_t
#include <string.h> // strerror


/// @note not used now
// static void sigHandler(int sig, siginfo_t *si, void *uc)
// {
//     TRACE_STATIC;
//
// }


struct sigaction& sigActionInit( struct sigaction &sigAct, const int signal )
{
  sigAct.sa_flags = SA_SIGINFO;
  //   sigAct.sa_sigaction = sigHandler;
  sigemptyset( &sigAct.sa_mask );
  sigaddset( &sigAct.sa_mask, signal );
  sigaction( signal, &sigAct, 0 );
  return sigAct;
}

sigset_t& sigSetInit( sigset_t &sigSet, const int signal )
{
  sigemptyset( &sigSet );
  sigaddset(&sigSet, signal );
  sigprocmask(SIG_SETMASK, &sigSet, NULL);
  return sigSet;
}


Timer::Timer( const int signal )
    : m_signal( signal )
    , m_sigAction( sigActionInit( m_sigAction , m_signal ) )
    , m_timerId( 0 )
    , m_periodic( false )
    , m_running( true )
    , m_mask( sigSetInit( m_mask, m_signal ) )
{
  TRACE;
}

Timer::~Timer()
{
  TRACE;

  sigprocmask(SIG_UNBLOCK, &m_mask, NULL);
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


  sigwaitinfo( &(m_sigAction.sa_mask), &sigInfo);
  tidp = (long*)sigInfo.si_value.sival_ptr;
//   LOG( Logger::FINEST, ( std::string( "Timer expired with ID: " ) +
//                          stringify( (timer_t)*tidp ) ).c_str() );

  timerExpired();

  if ( m_periodic ) {
    while ( m_running ) {

      sigwaitinfo( &(m_sigAction.sa_mask), &sigInfo);
      tidp = (long*)sigInfo.si_value.sival_ptr;
//       LOG( Logger::FINEST, ( std::string( "Timer expired with ID:" ) +
//                             stringify( (timer_t)*tidp ) ).c_str() );

      periodicTimerExpired();
    }
  }

}


void Timer::stopTimer()
{
  TRACE;

  struct itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  timer_settime( m_timerId, 0, &its, 0 );
  m_running = false;
}


void Timer::gracefulStop()
{
  TRACE;

  m_running = false;
}
