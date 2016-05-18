#ifndef PRINT_MESSAGE_HPP
#define PRINT_MESSAGE_HPP

#include <cpp_utils/Message.hpp>

/// @brief prints the received message
class PrintMessage : public Message
{
public:

  PrintMessage( void *msgParam = 0)
    : Message(msgParam)
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

    // threat m_params as "isready" flag
    *( static_cast<bool*>(m_param) ) = true;

    m_buffer.clear();
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

protected:

  size_t getExpectedLength()
  {
    TRACE;
    return 0;
  }

};

#endif // PRINT_MESSAGE_HPP
