//  gpp sslserver_main.cpp  -o sslserver -I../include ../src/Logger.cpp ../src/Socket.cpp -ggdb ../src/SocketServer.cpp ../src/Connection.cpp ../src/Poll.cpp  ../src/TcpConnection.cpp ../src/SslConnection.cpp  -lssl -lcrypto

#include "Logger.hpp"
#include "Common.hpp"

#include "Message.hpp"
#include "SslConnection.hpp"
#include "SocketServer.hpp"


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
                        append(TToStr(m_connection->getPort())) ).c_str() );

    std::string reply("Got your message, ");
    reply.append(m_connection->getHost()).append(":").
          append(TToStr(m_connection->getPort())).
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
  SslConnection::init();

  EchoMessage msg;
  SslConnection conn(argv[1], StrToT<int>(argv[2]), &msg);
  SocketServer socketServer(&conn);

  if ( !socketServer.start() ) {
    LOG( Logger::ERR, "Failed to start TCP server, exiting...");
    Logger::destroy();
    return 1;
  }

  // never reached
  sleep(1);

  socketServer.stop();
  SslConnection::destroy();
  Logger::destroy();
  return 0;
}