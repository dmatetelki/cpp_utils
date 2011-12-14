#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include "Connection.hpp"

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

    ~DummyConnection()
    {
      TRACE;
    }

    Connection* clone(const int)
    {
      TRACE;
      return 0;
    }


    bool bind()
    {
      TRACE;
      return true;
    }


    bool send(const void*, const size_t)
    {
      TRACE;
      return true;
    }

    bool receive()
    {
      TRACE;
      return true;
    }

    int getSocket() const
    {
      TRACE;
      return 0;
    }

  }; // DummyConnection


public:

  void testConnection()
  {
    DummyConnection c("localhost", "1234");

    TS_ASSERT_EQUALS (c.getHost() , std::string("localhost") );
    TS_ASSERT_EQUALS (c.getPort() , std::string("1234") );

    c.setHost("127.0.0.1");
    c.setPort("4455");

    TS_ASSERT_EQUALS (c.getHost() , std::string("127.0.0.1") );
    TS_ASSERT_EQUALS (c.getPort() , std::string("4455") );
  }

};
