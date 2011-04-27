#include "Timer.hpp"

#include "Common.hpp"

#include <signal.h> // sigset_t
#include <time.h>
#include <errno.h>

void notifyFunction(union sigval sigVal)
{
  TRACE_STATIC;

  ((TimerUser *)(sigVal.sival_ptr))->timerExpired();
}


Timer::Timer() : m_timerUsers()
{
  TRACE;
}

Timer::~Timer()
{
  TRACE;

  struct itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;

  std::map< timer_t, TimerUser* >::iterator it;
  for ( it = m_timerUsers.begin(); it != m_timerUsers.end(); it++ ) {
    timer_settime( it->first , 0, &its, 0 );
    it->second->timerDestroyed();
  }
  m_timerUsers.clear();
}


timer_t Timer::createTimer( TimerUser *timerUser,
                         const time_t interval_sec,
                         const long interval_nsec,
                         const time_t initExpr_sec,
                         const long initExpr_nsec )
{
  TRACE;

  // create timer
  struct sigevent sigev;
  timer_t timerId;

  sigev.sigev_notify = SIGEV_THREAD;
  sigev.sigev_value.sival_ptr = timerUser;
  sigev.sigev_notify_function = notifyFunction;
  sigev.sigev_notify_attributes = 0;

  /// @bug passing address of local variable
  timer_create( CLOCK_REALTIME, &sigev, &timerId );

  // arm it
  struct itimerspec its;
  its.it_value.tv_sec = interval_sec;
  its.it_value.tv_nsec = interval_nsec;
  its.it_interval.tv_sec = initExpr_sec;
  its.it_interval.tv_nsec = initExpr_nsec;

  timer_settime( timerId, 0, &its, 0 );
  m_timerUsers.insert( std::make_pair( timerId, timerUser ) );

  return timerId;
}


void Timer::stopTimer( timer_t timerId )
{
  TRACE;

  /// @bug why is this needed?
  timer_t tmp = timerId;

  // disarm timer
  struct itimerspec its;
  its.it_value.tv_sec = 0;
  its.it_value.tv_nsec = 0;
  if ( timer_settime( tmp, 0, &its, 0 ) == -1 ) {
    LOG( Logger::ERR, strerror( errno ) );
  }

  m_timerUsers[ tmp ]->timerDestroyed();
  m_timerUsers.erase( tmp );
}
