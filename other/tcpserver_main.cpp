// gpp tcpServer_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "Logger.hpp"
#include "Common.hpp"

#include "TcpServer.hpp"
#include "Message.hpp"

#include <iostream>
#include <string>

class EchoMessage : public Message<EchoMessage>
{
public:

  EchoMessage( Connection<EchoMessage>  *connection,
               void                     *msgParam = 0)
    : Message<EchoMessage>(connection, msgParam)
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

    LOG( Logger::INFO, std::string("Got message: \"").
                        append(m_buffer).append("\" from: ").
                        append(m_connection->getHost()).c_str() );

    std::string reply("Got your message, ");
    reply.append(m_connection->getHost()).
      append(" \"").append(m_buffer).append("\"");

    m_connection->send( reply.c_str(), reply.length() );
  }

protected:

  size_t getExpectedLength()
  {
    TRACE;
    return 0;
  }
};


int main( int argc, char * argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  TcpServer<EchoMessage> tcpServer("localhost", "4455");

  tcpServer.start();

  // never reached
  sleep(1);

  tcpServer.stop();
  Logger::destroy();
  return 0;
}