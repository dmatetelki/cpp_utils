#ifndef TIMER_USER_HPP
#define TIMER_USER_HPP

#include <time.h> // timer_t


class TimerUser
{
public:

  virtual void timerExpired() = 0;


protected:

  TimerUser(const clockid_t clockId = CLOCK_MONOTONIC);
  virtual ~TimerUser();

  bool startTimer(const time_t interval_sec,
                  const long interval_nsec = 0,
                  const time_t initExpr_sec = 0,
                  const long initExpr_nsec = 0);

  bool stopTimer();


private:

  TimerUser(const TimerUser&);
  TimerUser& operator=(const TimerUser&);

  timer_t m_timerId;

}; // class TimerUser


#endif // TIMMER_USER_HPP
