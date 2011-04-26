#ifndef TIMER_HPP
#define TIMER_HPP


#include <signal.h> // sigset_t
#include <time.h> // timer_t


class Timer
{

public:

  Timer( const int signal = SIGALRM );

  virtual ~Timer();
  virtual void timerExpired() {};
  virtual void periodicTimerExpired() {};

  void createTimer( const time_t interval_sec,
                    const long interval_nsec = 0,
                    const time_t initExpr_sec = 0,
                    const long initExpr_nsec = 0 );


  void wait();

  void stopTimer();

  void gracefulStop();


private:

  void notifyAndRemove( const timespec t );

  // after turning on all warnings, gcc reports that the class has pointer
  // data members (time_t, which is a long int by the way) so copy ctor and
  // assign op shall be declared
  Timer( const Timer& timer );
  Timer& operator=( const Timer& );

  int m_signal;
  timer_t m_timerId;
  bool m_periodic;
  bool m_running;

}; // class Timer


#endif // TIMER_HPP
