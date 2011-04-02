#include "Logger.hpp"

#include <time.h> //time
#include "Colors.hpp"


void Logger::init(std::ostream& log_stream )
{
  m_ostream = &log_stream;
}


void Logger::setLogLevel ( const LogLevel loglevel )
{
  m_logLevel = loglevel;
}



void Logger::log_pointer( const void* msg,
                          const char* file,
                          int line,
                          const char* function)
{
  *m_ostream  << getTime() << " "
              << COLOR( FG_GREEN ) << extractFilename(file)
              << COLOR_RESET << ":"
              << COLOR( FG_BROWN ) <<  line << COLOR_RESET << " "
              << COLOR( FG_CYAN ) << function << COLOR_RESET << " "
              << COLOR( FG_BLUE ) << "\"" << msg << "\""
              << COLOR_RESET << std::endl;
}


void Logger::log_string( const char* msg,
                         const char* file,
                         int line,
                         const char* function)
{
  *m_ostream  << getTime() << " "
              << COLOR( FG_GREEN ) << extractFilename(file)
              << COLOR_RESET << ":"
              << COLOR( FG_BROWN ) <<  line << COLOR_RESET << " "
              << COLOR( FG_CYAN ) << function << COLOR_RESET << " "
              << COLOR_F_FG( F_BOLD, FG_BROWN ) << "\"" << msg << "\""
              << COLOR_RESET << std::endl;
}

void Logger::msg(const char* text)
{
  *m_ostream  << COLOR_F( F_BOLD) << text << COLOR_RESET << std::endl;
}


Logger::LogLevel Logger::m_logLevel = Logger::FINEST;
std::ostream* Logger::m_ostream = 0;
