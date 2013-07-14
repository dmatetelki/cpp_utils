#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "Singleton.hpp"

#include <string>
#include <set>
#include <ostream>
#include "Common.hpp"


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
  static void usePrefix ( const bool use = true );

  inline static LogLevel getLoglevel() { return m_logLevel; }

  static void log_pointer( const void* pointer,
                           const char* file,
                           const int line,
                           const char* function);

  static void log_string( const int level,
                          const void* pointer,
                          const char* msg,
                          const char* file,
                          const int line,
                          const char* function);

  static void msg (const char* text);


private:

    Logger( const Logger& );
    Logger& operator=( const Logger& );

    static LogLevel m_logLevel;
    static std::ostream *m_ostream;
    static bool m_usePrefix;
};


#ifdef NO_TRACE

  #define TRACE           (void)0;
  #define TRACE_STATIC    (void)0;

#else

  #define TRACE \
    if ( Logger::getInstance()->getLoglevel() >= Logger::FINEST ) \
      Logger::getInstance()->log_pointer( \
        this, __FILE__, __LINE__, __PRETTY_FUNCTION__); \

  #define TRACE_STATIC \
    if ( Logger::getInstance()->getLoglevel() >= Logger::FINEST ) \
      Logger::getInstance()->log_pointer( \
        0, __FILE__, __LINE__, __PRETTY_FUNCTION__); \

#endif


#ifdef NO_LOGS

  #define LOG(level, msg) (void)0;
  #define LOG_STATIC(level, msg) (void)0;

  #define LOG_BEGIN(level) {
  #define LOG_PROP(name, value) (void)0;
  #define LOG_END(msg) }
  #define LOG_STATIC_END(msg) }

#else

  #define LOG(level, msg) \
    if ( Logger::getInstance()->getLoglevel() >= level ) \
      Logger::getInstance()->log_string( \
        level, this, msg, __FILE__, __LINE__, __PRETTY_FUNCTION__); \

  #define LOG_STATIC(level, msg) \
    if ( Logger::getInstance()->getLoglevel() >= level ) \
      Logger::getInstance()->log_string( \
        level, 0, msg, __FILE__, __LINE__, __PRETTY_FUNCTION__); \

  #define LOG_BEGIN(level) \
    if (Logger::getInstance()->getLoglevel() >= level) { \
      Logger::LogLevel loglevel(level); \
      std::string prop; \

  #define LOG_SPROP(variable) \
    prop.append(" ").append(#variable).append(":").append(TToStr(variable));

  #define LOG_PROP(name, value) \
    prop.append(" ").append(name).append(":").append(TToStr(value));

  #define LOG_END(msg) \
    std::string logline(msg); \
    logline.append(prop); \
    Logger::getInstance()->log_string(loglevel, this, logline.c_str(), \
      __FILE__, __LINE__, __PRETTY_FUNCTION__ ); }

  #define LOG_END_STATIC(msg) \
    std::string logline(msg); \
    logline.append(prop); \
    Logger::getInstance()->log_string(loglevel, 0, logline.c_str(), \
      __FILE__, __LINE__, __PRETTY_FUNCTION__ ); }

#endif


#define MSG(text) Logger::getInstance()->msg(text);


#endif // LOGGER_HPP
