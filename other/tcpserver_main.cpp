// gpp tcpServer_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "TcpServer.hpp"

#include "Logger.hpp"

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
                  const std::string msg)
  {
    TRACE;

    LOG( Logger::DEBUG, std::string("Got msg: ").append(msg).c_str() );
  }

};


int main( int argc, char * argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  EchoTcpServer tcpServer("localhost", "4455");

  tcpServer.start();

  sleep(10);

  tcpServer.stop();

  Logger::destroy();
  return 0;
}