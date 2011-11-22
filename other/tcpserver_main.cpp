// gpp tcpserver_main.cpp  -o server -I../include ../src/Logger.cpp ../src/Socket.cpp


#include "Logger.hpp"
#include "Common.hpp"

#include "TcpServer.hpp"
#include "Message.hpp"

#include <iostream>
#include <string>

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
    onMessageReady();
    return true;
  }

  void onMessageReady()
  {
    TRACE;

    LOG( Logger::INFO, std::string("Got message: \"").
                        append(m_buffer).append("\" from: ").
                        append(m_connection->getHost().append(":").
                        append(m_connection->getPort()) ).c_str() );

    std::string reply("Got your message, ");
    reply.append(m_connection->getHost()).append(":").
          append(m_connection->getPort()).
          append(" \"").append(m_buffer).append("\"");

    m_connection->send( reply.c_str(), reply.length() );
  }

  Message* clone()
  {
    TRACE;
    return new EchoMessage(m_param);
  }

protected:

  size_t getExpectedLength()
  {
    TRACE;
    return 0;
  }
};


int main(int argc, char* argv[] )
{
  if ( argc != 3 ) {
    std::cerr << "Usage: " << argv[0] <<  " <HOST> <PORT>" << std::endl;
    return 1;
  }

  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);
//   Logger::setNoPrefix();

  EchoMessage msg;

  TcpServer tcpServer( argv[1],
                       argv[2],
                       &msg );

  if ( !tcpServer.start() ) {
    LOG( Logger::ERR, "Failed to start TCP server, exiting...");
    Logger::destroy();
    return 1;
  }

  // never reached
  sleep(1);

  tcpServer.stop();
  Logger::destroy();
  return 0;
}