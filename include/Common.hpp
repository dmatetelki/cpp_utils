#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstring> // rindex
#include <string> // string

#include "Logger.hpp"


#include <time.h> // timespec, CLOCK_REALTIME

#include <typeinfo> //typeid
#include <stdexcept> // runtime_error
#include <sstream> // ostringstream

#include <errno.h> // errno
#include <string.h> // strerror


const long NANO = 1000000000L;  // 10^9


inline timespec addTotimespec( const long int & sec, const long int & nsec)
{

  timespec abs_time;
  clock_gettime ( CLOCK_REALTIME, &abs_time );
  long int nsecSum( abs_time.tv_nsec + nsec );

  if ( nsecSum >= NANO ) {
    abs_time.tv_sec += sec + nsecSum / NANO;
    abs_time.tv_nsec = nsecSum % NANO;
  } else {
    abs_time.tv_sec += sec;
    abs_time.tv_nsec += nsec;
  }
  return abs_time;
}


inline timespec timespecAdd( timespec& t1, const timespec& t2 )
{
  timespec  result;
  result.tv_sec = t1.tv_sec + t2.tv_sec ;
  result.tv_nsec = t1.tv_nsec + t2.tv_nsec ;
  if (result.tv_nsec >= NANO ) {
    result.tv_sec++ ;
    result.tv_nsec = result.tv_nsec - NANO;
  }
  return result;
}

inline timespec timespecSubstract( timespec& t1, const timespec& t2 )
{
  timespec  result;
  if ((t1.tv_sec < t2.tv_sec) ||
      ((t1.tv_sec == t2.tv_sec) && (t1.tv_nsec <= t2.tv_nsec))) {
    result.tv_sec = result.tv_nsec = 0 ;
  } else {
    result.tv_sec = t1.tv_sec - t2.tv_sec ;
    if (t1.tv_nsec < t2.tv_nsec) {
      result.tv_nsec = t1.tv_nsec + NANO - t2.tv_nsec ;
      result.tv_sec-- ;
    } else {
      result.tv_nsec = t1.tv_nsec - t2.tv_nsec ;
    }
  }
  return result;
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


template <class T>
inline std::string TToStr(const T t)
{
  std::ostringstream oss;
  oss << t;
  return oss.str();
}


// template <class T>
// inline void StrToT( T &t, const std::string s )
// {
//   std::stringstream ss(s);
//   ss >> t;
// }

template <class T>
inline T StrToT( const std::string s )
{
  std::stringstream ss(s);
  T t;
  ss >> t;
  return t;
}


inline std::string errnoToString(const char *s)
{
  return std::string(s).append(strerror(errno));
}

#endif // COMMON_HPP
