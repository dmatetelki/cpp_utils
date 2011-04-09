#include "Timer.hpp"

#include "Common.hpp"

#include <signal.h> // sigset_t
#include <time.h> // timer_t
#include <string.h> // strerror

Timer::Timer(const int signal)
    : m_signal(signal)
    , m_periodic(false)
    , m_running(true)
{
  TRACE;

  m_sigAction.sa_flags = SA_SIGINFO;
  sigemptyset(&m_sigAction.sa_mask);
  sigaddset( &m_sigAction.sa_mask, m_signal );
  sigaction( m_signal, &m_sigAction, 0 );
}


void Timer::createTimer(const time_t interval_sec,
                        const long interval_nsec,
                        const time_t initExpr_sec,
                        const long initExpr_nsec)
{
  TRACE;

  // create timer
  struct sigevent sigev;
  sigev.sigev_notify = SIGEV_SIGNAL;
  sigev.sigev_signo = m_signal;
  sigev.sigev_value.sival_ptr = &m_timerId;
  if ( timer_create( CLOCK_MONOTONIC, &sigev, &m_timerId ) == -1 ) {
    LOG ( Logger::FINEST, "Error from timer_create: " /*strerror(errno)*/ );
    return;
  }

  // arm it
  struct itimerspec its;
  its.it_value.tv_sec = interval_sec;
  its.it_value.tv_nsec = interval_nsec;
  its.it_interval.tv_sec = initExpr_sec;
  its.it_interval.tv_nsec = initExpr_nsec;

  if ( initExpr_sec != 0 or initExpr_nsec != 0 ) m_periodic = true;

  if ( timer_settime( m_timerId, 0, &its, 0 ) == -1 ) {
        LOG ( Logger::FINEST, "Error from timer_settime: " /*strerror(errno)*/ );
    return;
  }
}


void Timer::wait()
{
  TRACE;

  int sig;
  sigwait( &m_sigAction.sa_mask, &sig );
  timerExpired();
  if ( m_periodic ) {
    while ( m_running ) {
      sigwait( &m_sigAction.sa_mask, &sig );
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
