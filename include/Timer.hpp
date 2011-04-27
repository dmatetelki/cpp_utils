#ifndef TIMER_HPP
#define TIMER_HPP


#include <signal.h> // sigset_t
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
                       const time_t interval_sec,
                       const long interval_nsec = 0,
                       const time_t initExpr_sec = 0,
                       const long initExpr_nsec = 0 );


  void stopTimer( timer_t timerId );


private:

  std::map< timer_t, TimerUser* > m_timerUsers;

}; // class Timer


#endif // TIMER_HPP
