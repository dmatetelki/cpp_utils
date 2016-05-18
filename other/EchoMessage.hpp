#ifndef EchoMessage_HPP
#define EchoMessage_HPP

#include <cpp_utils/Message.hpp>

/// @brief echoes back the message to the sender
class EchoMessage : public Message
{
public:

  EchoMessage( void *msgParam = 0)
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

    m_connection->send(m_buffer.c_str(), m_buffer.length());
    m_buffer.clear();
  }

  Message* clone()
  {
    TRACE;
    return new EchoMessage(m_param);
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

#endif // EchoMessage_HPP
