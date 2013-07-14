#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include <cpp_utils/StreamConnection.hpp>

class TestStreamConnection : public CxxTest::TestSuite
{
private:

  class DummyStreamConnection : public StreamConnection
  {
  public:

    DummyStreamConnection(const std::string host, const std::string port)
      : StreamConnection(host, port)
    {
      TRACE;
    }

    virtual ~DummyStreamConnection()
    {
      TRACE;
    }

    Connection* clone(const int) { return 0; }
    bool bind() { return true; }
    bool send(const void*, const size_t) { return true; }
    bool receive() { return true; }
    int getSocket() const { return 0; }

    bool connect() { return true; }
    bool disconnect() { return true; }
    bool listen(const int) { return true; }
    bool accept(int) { return true; }
    bool closed() const { return true; }

  }; // StreamConnection


public:

  void testStreamConnection()
  {
    TEST_HEADER;

    DummyStreamConnection c("localhost", "1234");

    TS_ASSERT_EQUALS (c.getHost() , std::string("localhost") );
    TS_ASSERT_EQUALS (c.getPort() , std::string("1234") );

    c.setHost("127.0.0.1");
    c.setPort("4455");

    TS_ASSERT_EQUALS (c.getHost() , std::string("127.0.0.1") );
    TS_ASSERT_EQUALS (c.getPort() , std::string("4455") );
  }

};
