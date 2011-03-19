#ifndef COMMON_HPP
#define COMMON_HPP


#include <iostream>

#ifndef NOTRACE
  #define TRACE(x) std::cout << x << " " << __PRETTY_FUNCTION__ <<  \
                          " : " << __LINE__ << std::endl

#else
  /// @todo Get rid of the "warning: statement has no effect" compiler msgs
  #define TRACE(x) ""
#endif


#define STRINGIZE(c) #c


#include <time.h>

inline timespec intIntervalTotimespec(const int & interval)
{
  timespec abs_time;
  clock_gettime ( CLOCK_REALTIME, &abs_time );
  abs_time.tv_nsec += interval * 1000000;
  if ( abs_time.tv_nsec >= 1000000000 ) {
    abs_time.tv_nsec -= 1000000000;
    abs_time.tv_sec += 1;
  }
  return abs_time;
};


#endif // COMMON_HPP
