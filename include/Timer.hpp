#ifndef TIMER_HPP
#define TIMER_HPP

#include <signal.h> // sigset_t
#include <time.h> // timer_t

class Timer
{

public:

  Timer(const int signal = SIGALRM );


  virtual void timerExpired()  {}

  virtual void periodicTimerExpired() {}


  void createTimer(const time_t interval_sec,
                      const long interval_nsec = 0,
                      const time_t initExpr_sec = 0,
                      const long initExpr_nsec = 0);

  void wait();

  void stopTimer();

private:

  int m_signal;
  struct sigaction m_sigAction;
  timer_t m_timerId;
  bool m_periodic;
  bool m_running;

}; // class Timer


#endif // TIMER_HPP
