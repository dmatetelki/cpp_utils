#include "Logger.hpp"

#include <time.h> //time
#include "Colors.hpp"

#include "Common.hpp"


void Logger::init(std::ostream& log_stream )
{
  m_ostream = &log_stream;
}


void Logger::setLogLevel ( const LogLevel loglevel )
{
  m_logLevel = loglevel;
}


void Logger::setNoPrefix ()
{
  m_usePrefix = false;
}


void Logger::log_pointer( const void* pointer,
                          const char* file,
                          const int line,
                          const char* function)
{
  if ( !m_usePrefix ) {
    *m_ostream << pointer << std::endl;
    return;
  }

  *m_ostream  << getTime() << " "
              << COLOR( FG_GREEN ) << extractFilename(file)
              << COLOR_RESET << ":"
              << COLOR( FG_BROWN ) <<  line << COLOR_RESET << " "
              << COLOR( FG_CYAN ) << function << COLOR_RESET << " "
              << COLOR( FG_BLUE ) << "\"" << pointer << "\""
              << COLOR_RESET << std::endl;
}


void Logger::log_string( const int level,
                         const void* pointer,
                         const char* msg,
                         const char* file,
                         const int line,
                         const char* function)
{
  if ( !m_usePrefix ) {
    *m_ostream << msg << std::endl;
    return;
  }

  const char *color;
  if ( level <= WARNING ) { color = COLOR_F_FG( F_BOLD, FG_RED ); }
  else if ( level <= INFO ) { color = COLOR_F_FG( F_BOLD, FG_WHITE); }
  else { color = COLOR_F_FG( F_BOLD, FG_BROWN); }

  *m_ostream  << getTime() << " "
              << COLOR( FG_GREEN ) << extractFilename(file)
              << COLOR_RESET << ":"
              << COLOR( FG_BROWN ) <<  line << COLOR_RESET << " "
              << COLOR( FG_CYAN ) << function << COLOR_RESET << " "
              << color << "\"" << msg << "\""
              << COLOR( FG_BLUE ) << "\"" << pointer << "\""
              << COLOR_RESET << std::endl;
}

void Logger::msg(const char* text)
{
  *m_ostream  << COLOR_F( F_BOLD) << text << COLOR_RESET << std::endl;
}


Logger::LogLevel Logger::m_logLevel = Logger::FINEST;
std::ostream* Logger::m_ostream = 0;
bool Logger::m_usePrefix = true;
