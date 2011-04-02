#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstring> // rindex
#include <string> // string

#include "Logger.hpp"


#include <time.h> // timespec, CLOCK_REALTIME

#include <typeinfo> //typeid
#include <stdexcept> // runtime_error
#include <sstream> // ostringstream

inline timespec addTotimespec(const long int & sec, const long int & nsec)
{
  const int nano = 1000000000;  // 10^9
  timespec abs_time;
  clock_gettime ( CLOCK_REALTIME, &abs_time );
  long int nsecSum( abs_time.tv_nsec + nsec );

  if ( nsecSum >= nano ) {
    abs_time.tv_sec += sec + nsecSum / nano;
    abs_time.tv_nsec = nsecSum % nano;
  } else {
    abs_time.tv_sec += sec;
    abs_time.tv_nsec += nsec;
  }
  return abs_time;
}


inline const char* extractFilename( const char *path )
{
  char const * f = rindex(path, '/');
  if (f) return ++f;
  return path;
}


template<typename T>
inline std::string stringify(T const& x)
{
  std::ostringstream o;
  o << x;
  return o.str();
}


inline std::string getTime( void )
{
  time_t tim = time( NULL );
  struct tm * now = localtime( &tim );

  std::string ret = stringify(now->tm_hour) + ":" +
                    stringify(now->tm_min) + ":" +
                    stringify(now->tm_sec);

  return ret;
}

#endif // COMMON_HPP
