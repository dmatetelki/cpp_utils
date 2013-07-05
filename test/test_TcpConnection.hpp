#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include "Thread.hpp"
#include "TcpConnection.hpp"
#include "test_Message.hpp" // SimpleMessage

#include "SocketClient.hpp"

class TestTcpConnection : public CxxTest::TestSuite
{
private:

  class TcpServerThread : public Thread
                        , public TcpConnection
  {
  public:

    TcpServerThread(const std::string   host,
                    const std::string   port,
                    Message            *message,
                    const size_t        bufferLength = 1024)
      : TcpConnection(host, port, message, bufferLength)
    {
      TRACE;
    }

  private:

    void* run( void )
    {
      TRACE;

      while (m_isRunning)
        sleep(1);

      return 0;
    }


  }; // TcpServerThread

class DummyTcpClient : public SocketClient
{

};

public:

  void testBindToTheSamePort()
  {
    TEST_HEADER;

    SimpleMessage message;
    TcpServerThread tcpServerThread1("localhost", "1234", &message);
    TcpServerThread tcpServerThread2("localhost", "1234", &message);

    TS_ASSERT_EQUALS(tcpServerThread1.bind(), true);
    TS_ASSERT_EQUALS(tcpServerThread2.bind(), false);

    TS_ASSERT_EQUALS(tcpServerThread1.disconnect(), false);
    TS_ASSERT_EQUALS(tcpServerThread2.disconnect(), false);
  }

  void testListen()
  {
    TEST_HEADER;

    SimpleMessage message;
    TcpServerThread tcpServerThread1("localhost", "1235", &message);

    TS_ASSERT_EQUALS(tcpServerThread1.bind(), true);
    TS_ASSERT_EQUALS(tcpServerThread1.listen(), true);

    TS_ASSERT_EQUALS(tcpServerThread1.closed(), false);

    TS_ASSERT_EQUALS(tcpServerThread1.disconnect(), true);
  }

  void testBasicCommunication()
  {
    TEST_HEADER;

    SimpleMessage message;
    std::string host("localhost");
    std::string service("1234");

    TcpServerThread tcpServerThread1(host, service, &message);

    TS_ASSERT_EQUALS(tcpServerThread1.bind(), true);
    tcpServerThread1.start();

    TcpConnection conn(host, service, &message);
    SocketClient socketClient(&conn);

    TS_ASSERT_EQUALS(socketClient.connect(), true );

    // wait for thread creation
    sleep(1);

    // send message to server
    std::string msg1("hello");
    TS_ASSERT_EQUALS(socketClient.send( msg1.c_str(), msg1.length()), true);

    // wait for the complate &handled reply
    struct timespec tm = {0,1000};
    while ( !finished && socketClient.isPolling() )
            nanosleep(&tm, &tm) ;

    socketClient.disconnect();

    TS_ASSERT_EQUALS(tcpServerThread2.bind(), false);

    TS_ASSERT_EQUALS(tcpServerThread1.disconnect(), false);
    TS_ASSERT_EQUALS(tcpServerThread2.disconnect(), false);
  }

};
