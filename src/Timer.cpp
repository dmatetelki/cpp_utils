#include "Timer.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <errno.h>
#include <signal.h> // sigevent



void notifyFunction(union sigval sigVal)
{
  TRACE_STATIC;

  reinterpret_cast<TimerUser*>(sigVal.sival_ptr)->timerExpired();
}


timer_t Timer::createTimer( TimerUser *timerUser,
                            const clockid_t clockId )
{
  TRACE_STATIC;

  sigevent sigev;
  timer_t timerId(0);

  sigev.sigev_notify = SIGEV_THREAD;
  sigev.sigev_value.sival_ptr = timerUser;
  sigev.sigev_notify_function = notifyFunction;
  sigev.sigev_notify_attributes = 0;

  if (timer_create(clockId, &sigev, &timerId) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END_STATIC("Could create timer.");
    return 0;
  }

  return timerId;
}


bool Timer::setTimer( timer_t &timerId,
                      const time_t interval_sec,
                      const long interval_nsec,
                      const time_t initExpr_sec,
                      const long initExpr_nsec )
{
  TRACE_STATIC;

  if (timerId == 0)
    return false;

  itimerspec its;
  its.it_value.tv_sec = interval_sec;
  its.it_value.tv_nsec = interval_nsec;
  its.it_interval.tv_sec = initExpr_sec;
  its.it_interval.tv_nsec = initExpr_nsec;

  if (timer_settime( timerId, 0, &its, NULL ) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END_STATIC("Could set timer.");
    return false;
  }

  return true;
}


bool Timer::deleteTimer(timer_t &timerId)
{
  TRACE_STATIC;

  if (timerId == 0)
    return false;

  if (timer_delete(timerId) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END_STATIC("Could delete timer.");
    return false;
  }

  timerId = 0;
  return false;
}
