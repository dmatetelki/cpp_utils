#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstring> // rindex
#include <string> // string

#include "Logger.hpp"


#include <time.h> // timespec, CLOCK_REALTIME

#include <typeinfo> //typeid
#include <stdexcept> // runtime_error
#include <sstream> // ostringstream

inline timespec addSecTotimespec(const long int & sec)
{
  timespec abs_time;
  clock_gettime ( CLOCK_REALTIME, &abs_time );
  abs_time.tv_sec += sec;
  return abs_time;
}


inline const char* extractFilename( const char *path )
{
  char const * f = rindex(path, '/');
  if (f) return ++f;
  return path;
}


class BadConversion : public std::runtime_error
{

public:

  BadConversion(std::string const& s) : std::runtime_error(s) { }

};


template<typename T>
inline std::string stringify(T const& x)
{
  std::ostringstream o;
  if (!(o << x))
    throw BadConversion(std::string("stringify(") + typeid(x).name() + ")");
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
