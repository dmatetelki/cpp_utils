#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include "Message.hpp"
#include "Connection.hpp"

class TestMessage : public CxxTest::TestSuite
{
private:

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

  }; // SimpleMessage


  class DummyConnection : public Connection
  {
  public:

    DummyConnection(const std::string host,
                    const std::string port,
                    Message *message)
      : Connection(host, port)
      , m_message(message)
      , m_receiveBuffer()
      , m_sendBuffer()
    {
      TRACE;
    }

    virtual ~DummyConnection()
    {
      TRACE;
    }

    Connection* clone(const int) { return 0; }
    bool bind() { return true; }
    bool send(const void* msg, const size_t len)
    {
      TRACE;
      m_sendBuffer = std::string( (char*)msg, len);
      return true;
    }

    bool receive()
    {
      TRACE;
      m_message->buildMessage(m_receiveBuffer.c_str(), m_receiveBuffer.length());
      return true;
    }

    int getSocket() const { return 0; }

    void setReceiveBuffer(const std::string msg)
    {
      TRACE;
      m_receiveBuffer = msg;
    }

    std::string getSendBuffer()
    {
      TRACE;
      return m_sendBuffer;
    }

  private:

    DummyConnection(const TestMessage::DummyConnection&);
    DummyConnection& operator=(const DummyConnection&);

    Message *m_message;
    std::string m_receiveBuffer;
    std::string m_sendBuffer;

  }; // DummyConnection


public:

  void testSimpleMessage()
  {
    TEST_HEADER;

    bool param(false);
    SimpleMessage simpleMessage(&param);

    std::string msg("hello");
    simpleMessage.buildMessage(msg.c_str(), msg.length());
    TS_ASSERT_EQUALS(simpleMessage.getBuffer(), msg);
    TS_ASSERT_EQUALS(param, true);
  }

  void testConnectionWithSimpleMessage()
  {
    TEST_HEADER;

    bool param(false);
    SimpleMessage simpleMessage(&param);

    DummyConnection dummyConnection("localhost", "1234", &simpleMessage);

    std::string msg("hello");
    dummyConnection.setReceiveBuffer(msg);
    dummyConnection.receive();

    TS_ASSERT_EQUALS(simpleMessage.getBuffer(), msg);
    TS_ASSERT_EQUALS(param, true);
  }

  void testInteraction()
  {
    TEST_HEADER;

    bool param(false);
    SimpleMessage simpleMessage(&param);

    DummyConnection dummyConnection("localhost", "1234", &simpleMessage);
    simpleMessage.setConnection(&dummyConnection);

    std::string msg("hello");
    dummyConnection.setReceiveBuffer(msg);
    dummyConnection.receive();

    TS_ASSERT_EQUALS(simpleMessage.getBuffer(), msg);
    TS_ASSERT_EQUALS(param, true);

    TS_ASSERT_EQUALS(dummyConnection.getSendBuffer(), msg);
  }

};
