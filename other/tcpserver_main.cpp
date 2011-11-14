// gpp tcpServer_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "TcpServer.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <iostream>
#include <string>

class EchoTcpServer : public TcpServer
{
public:

  EchoTcpServer ( const std::string host,
                  const std::string port,
                  const int maxClients = 5 )
    : TcpServer(host, port, maxClients)
  {
    TRACE;
  }

  void msgArrived(const int clientSocket,
                  const unsigned char*msg,
                  const int msgLen )
  {
    TRACE;

    std::string message((char*)msg, msgLen);
    LOG( Logger::DEBUG, std::string("Got msg: ").append(message).c_str() );

    std::string reply("Got your msg, buddy: \"");
    reply.append(message).append("\" see you!");

    ssize_t n = write(clientSocket,message.c_str(), message.length());
    if (n == -1) {
      LOG( Logger::ERR, errnoToString("ERROR writing to socket. ").c_str() );
    }
  }

};


int main( int argc, char * argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  EchoTcpServer tcpServer("localhost", "4455");

  tcpServer.start();

  // never reached
  sleep(1);

  tcpServer.stop();

  Logger::destroy();
  return 0;
}