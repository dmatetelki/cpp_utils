#include "Logger.hpp"

#include <time.h> //time


void Logger::init(std::ostream& log_stream )
{
  m_ostream = &log_stream;
}


void Logger::setLogLevel ( const LogLevel loglevel )
{
  m_logLevel = loglevel;
}



void Logger::log_pointer( const LogLevel loglevel,
                  const void* msg,
                  const char* file,
                  int line,
                  const char* function)
{
  *m_ostream  << getTime() << " "
              << extractFilename(file) << ":"
              << line << " "
              << function << " "
              << "\"" << msg << "\""
              << std::endl;
}


void Logger::log_string( const LogLevel loglevel,
                  const char* msg,
                  const char* file,
                  int line,
                  const char* function)
{
  *m_ostream  << getTime() << " "
              << extractFilename(file) << ":"
              << line << " "
              << function << " "
              << "\"" << msg << "\""
              << std::endl;
}

void Logger::msg(const char* text)
{
  *m_ostream  << text << std::endl;
}


Logger::LogLevel Logger::m_logLevel = Logger::FINEST;
std::ostream* Logger::m_ostream = 0;
