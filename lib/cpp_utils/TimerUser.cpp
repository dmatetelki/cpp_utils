#include "TimerUser.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "Timer.hpp"

#include <errno.h>


TimerUser::TimerUser(const clockid_t clockId)
  : m_timerId(Timer::createTimer(this, clockId))
{
  TRACE;
}


TimerUser::~TimerUser()
{
  TRACE;

  Timer::deleteTimer(m_timerId);
}


bool TimerUser::startTimer(const time_t interval_sec,
                           const long interval_nsec,
                           const time_t initExpr_sec,
                           const long initExpr_nsec)
{
  TRACE;

  return Timer::setTimer(m_timerId, interval_sec, interval_nsec, initExpr_sec, initExpr_nsec);
}


bool TimerUser::stopTimer()
{
  TRACE;

  return Timer::setTimer(m_timerId, 0);
}
