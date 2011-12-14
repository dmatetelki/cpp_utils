#ifndef TIMER_HPP
#define TIMER_HPP

#include "TimerUser.hpp"

#include <time.h> // timer_t


class Timer
{

public:

  static timer_t createTimer( TimerUser *timerUser,
                              const clockid_t clockId = CLOCK_MONOTONIC );

  static bool setTimer( timer_t &timerId,
                        const time_t interval_sec,
                        const long interval_nsec = 0,
                        const time_t initExpr_sec = 0,
                        const long initExpr_nsec = 0 );

  static bool deleteTimer(timer_t &timerId);

}; // class Timer


#endif // TIMER_HPP
