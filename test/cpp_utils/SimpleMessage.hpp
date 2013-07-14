#ifndef SIMPLEMESSAGE_HPP
#define SIMPLEMESSAGE_HPP

#include <cpp_utils/Message.hpp>

class SimpleMessage : public Message
{
public:

  SimpleMessage( void *msgParam = 0)
    : Message(msgParam)
  {
    TRACE;
  }

  bool buildMessage( const void   *msgPart,
                     const size_t  msgLen )
  {
    TRACE;
    m_buffer = std::string( (const char*) msgPart, msgLen );
    onMessageReady();
    return true;
  }

  void onMessageReady()
  {
    TRACE;

    LOG_BEGIN(Logger::INFO)
      LOG_PROP("message", m_buffer)
    LOG_END("Got reply from server.");

    *( static_cast<bool*>(m_param) ) = true;

    if (m_connection != 0)
      m_connection->send(m_buffer.c_str(), m_buffer.length());

    m_buffer.clear();
  }

  Message* clone()
  {
    TRACE;
    return new SimpleMessage(m_param);
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

#endif // SIMPLEMESSAGE_HPP
