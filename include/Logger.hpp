#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Singleton.hpp"

#include <string>
#include <set>
#include <ostream>


class Logger : public Singleton<Logger>
{

public:

  enum LogLevel {
    EMERG = 0,  // system is unusable
    ALERT,      // action must be taken immediately
    CRIT,       // critical conditions
    ERR,        // error conditions
    WARNING,    // warning conditions
    NOTICE,     // normal but significant condition
    INFO,       // informational
    DEBUG,      // debug-level messages
    FINEST
  };

  Logger() {}
  virtual ~Logger() { m_ostream->flush(); }

  static void init(std::ostream& log_stream );

  static void setLogLevel ( const LogLevel loglevel );
  inline static LogLevel getLoglevel() { return m_logLevel; }

  static void log_pointer( const void* msg,
                           const char* file,
                           int line,
                           const char* function);

  static void log_string( const char* msg,
                          const char* file,
                          int line,
                          const char* function);

  static void msg (const char* text);


private:

    Logger( const Logger& );
    Logger& operator=( const Logger& );

    static LogLevel m_logLevel;
    static std::ostream *m_ostream;
};


#ifdef NO_TRACE
  #define MAX_LOFLEVEL Logger::DEBUG
#else
  #define MAX_LOFLEVEL Logger::FINEST
#endif


#define TRACE \
if(MAX_LOFLEVEL >= Logger::FINEST && \
  Logger::getInstance()->getLoglevel() >= Logger::FINEST ) \
Logger::getInstance()->log_pointer( \
  this, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
else (void)0


#define LOG(level, msg) \
if (MAX_LOFLEVEL >= level && \
  Logger::getInstance()->getLoglevel() >= Logger::FINEST ) \
Logger::getInstance()->log_string( \
  msg, __FILE__, __LINE__, __PRETTY_FUNCTION__); \
else (void)0


/// @todo remove this
#define MSG(text) Logger::getInstance()->msg(text)


#endif // LOGGER_HPP
