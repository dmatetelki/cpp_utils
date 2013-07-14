#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include <cpp_utils/Connection.hpp>

class TestConnection : public CxxTest::TestSuite
{
private:

  class DummyConnection : public Connection
  {
  public:

    DummyConnection(const std::string host, const std::string port)
      : Connection(host, port)
    {
      TRACE;
    }

    virtual ~DummyConnection()
    {
      TRACE;
    }

    Connection* clone(const int) { return 0; }
    bool bind() { return true; }
    bool send(const void*, const size_t) { return true; }
    bool receive() { return true; }
    int getSocket() const { return 0; }

  }; // DummyConnection


public:

  void testConnection()
  {
    TEST_HEADER;

    DummyConnection c("localhost", "1234");

    TS_ASSERT_EQUALS (c.getHost() , std::string("localhost") );
    TS_ASSERT_EQUALS (c.getPort() , std::string("1234") );

    c.setHost("127.0.0.1");
    c.setPort("4455");

    TS_ASSERT_EQUALS (c.getHost() , std::string("127.0.0.1") );
    TS_ASSERT_EQUALS (c.getPort() , std::string("4455") );
  }

};
