#include "Timer.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <signal.h> // sigset_t
#include <time.h>
#include <errno.h>

void notifyFunction(union sigval sigVal)
{
  TRACE_STATIC;

  reinterpret_cast<TimerUser*>(sigVal.sival_ptr)->timerExpired();
}


Timer::Timer()
  : m_timerUsers()
{
  TRACE;
}

Timer::~Timer()
{
  TRACE;

  for (TimerUserMap::iterator it = m_timerUsers.begin(); it != m_timerUsers.end(); )
    stopTimer((it++)->first);

  m_timerUsers.clear();
}


timer_t Timer::createTimer( TimerUser *timerUser,
                             clockid_t clockId )
{
  TRACE;

  sigevent sigev;
  timer_t timerId(0);

  sigev.sigev_notify = SIGEV_THREAD;
  sigev.sigev_value.sival_ptr = timerUser;
  sigev.sigev_notify_function = notifyFunction;
  sigev.sigev_notify_attributes = 0;

  if (timer_create(clockId, &sigev, &timerId) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could create timer.");
    return 0;
  }

  if (m_timerUsers.find(timerId) != m_timerUsers.end() ) {
    LOG_BEGIN(Logger::ERR)
      LOG_SPROP(timerId)
    LOG_END("TimerId already in map.");
    return 0;
  }

  m_timerUsers.insert(std::make_pair(timerId, timerUser));
  return timerId;
}


bool Timer::setTimer( timer_t timerId,
                      const time_t interval_sec,
                      const long interval_nsec,
                      const time_t initExpr_sec,
                      const long initExpr_nsec )
{
  if (m_timerUsers.find(timerId) == m_timerUsers.end() ) {
    LOG_BEGIN(Logger::ERR)
      LOG_SPROP(timerId)
    LOG_END("TimerId is not in the map.");
    return false;
  }

  itimerspec its;
  its.it_value.tv_sec = interval_sec;
  its.it_value.tv_nsec = interval_nsec;
  its.it_interval.tv_sec = initExpr_sec;
  its.it_interval.tv_nsec = initExpr_nsec;

  if (timer_settime( timerId, 0, &its, NULL ) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could set timer.");
    return false;
  }
  return true;
}


bool Timer::stopTimer( timer_t timerId )
{
  TRACE;

  if (m_timerUsers.find(timerId) == m_timerUsers.end() ) {
    LOG_BEGIN(Logger::ERR)
      LOG_SPROP(timerId)
    LOG_END("TimerId is not in the map.");
    return false;
  }

  setTimer(timerId, 0);
  m_timerUsers[timerId]->timerDestroyed();
  m_timerUsers.erase(timerId);
  return true;
}
