#ifndef PRINT_MESSAGE_HPP
#define PRINT_MESSAGE_HPP

#include <cpp_utils/Message.hpp>

#include <mutex>
#include <condition_variable>


/// @brief prints the received message, can signal when ready
class PrintMessage : public Message
{
  struct WaitForFlag {
    bool m_isReady;
    std::condition_variable m_condition_variable;
    std::mutex m_mutex;
    WaitForFlag() : m_isReady(false), m_condition_variable(), m_mutex() {}
  };

public:

  PrintMessage( void *msgParam = 0)
    : Message(msgParam)
    , m_flag()
  {
    TRACE;
  }

  bool buildMessage( const void   *msgPart,
                     const size_t  msgLen )
  {
    TRACE;
    m_buffer = std::string( (const char*) msgPart, msgLen );

    // not using getExpectedLength
    onMessageReady();
    return true;
  }

  void onMessageReady()
  {
    TRACE;

    LOG_BEGIN(Logger::INFO)
      LOG_PROP("message", m_buffer)
      LOG_PROP("host", m_connection->getHost())
      LOG_PROP("port", m_connection->getPort())
    LOG_END("Got message.");

    m_buffer.clear();
    std::unique_lock<std::mutex> lock(m_flag.m_mutex);
    m_flag.m_isReady = true;
    m_flag.m_condition_variable.notify_one();
  }

  Message* clone()
  {
    TRACE;
    return new PrintMessage(m_param);
  }

  std::string getBuffer()
  {
    TRACE;
    return m_buffer;
  }

  void waitForReady() {
    std::unique_lock<std::mutex> lock(m_flag.m_mutex);
    while (!m_flag.m_isReady)
      m_flag.m_condition_variable.wait(lock);
  }

protected:

  size_t getExpectedLength()
  {
    TRACE;
    return 0;
  }

private:

  WaitForFlag m_flag;
};

#endif // PRINT_MESSAGE_HPP
