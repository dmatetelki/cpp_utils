#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include <cpp_utils/Message.hpp>
#include "SimpleMessage.hpp"
#include <cpp_utils/Connection.hpp>


class TestMessage : public CxxTest::TestSuite
{
private:

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
