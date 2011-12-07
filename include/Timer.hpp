#ifndef TIMER_HPP
#define TIMER_HPP


#include <time.h> // timer_t
#include <map>

class TimerUser
{

public:

  virtual void timerExpired() = 0;
  virtual void timerDestroyed() = 0;

  virtual ~TimerUser() {}

}; // class TimerUser


class Timer
{

public:

  Timer();

  virtual ~Timer();

  timer_t createTimer( TimerUser *m_timerUser,
                       clockid_t clockId = CLOCK_MONOTONIC );

  bool setTimer( timer_t timerId,
                 const time_t interval_sec,
                 const long interval_nsec = 0,
                 const time_t initExpr_sec = 0,
                 const long initExpr_nsec = 0 );

  bool stopTimer( timer_t timerId );


private:

  typedef std::map<timer_t, TimerUser*> TimerUserMap;

  TimerUserMap m_timerUsers;

}; // class Timer


#endif // TIMER_HPP
