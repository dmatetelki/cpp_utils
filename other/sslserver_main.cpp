// run with
// ./sslserver  127.0.0.1 9001  ./cert.pem ./key.pem


#include <cpp_utils/Logger.hpp>
#include <cpp_utils/Common.hpp>
#include <cpp_utils/SslConnection.hpp>
#include <cpp_utils/SocketServer.hpp>

#include "EchoMessage.hpp"

#include <iostream>
#include <string>

#include <signal.h>


SocketServer *socketServer;

void signalHandler(int s)
{
  LOG_STATIC( Logger::INFO, std::string("Exiting after receiving signal: ").
                              append(TToStr(s)).c_str() );
  socketServer->stop();
  delete socketServer;
  SslConnection::destroy();
  Logger::destroy();
  exit(1);
}


int main(int argc, char* argv[] )
{
  if ( argc != 5 ) {
    std::cerr << "Usage: " << argv[0] <<  " <HOST> <PORT> <CERT> <PRIVKEY>" << std::endl;
    return 1;
  }

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = signalHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);


  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);
  SslConnection::init();

  EchoMessage msg;
  SslConnection conn(argv[1], argv[2], &msg);
  if ( !conn.initServerContext(argv[3], argv[4]) ) {
    LOG_STATIC( Logger::ERR, "Failed to init SSL context, exiting...");
    SslConnection::destroy();
    Logger::destroy();
    return 1;
  }

  socketServer = new SocketServer(&conn);

  if ( !socketServer->start() ) {
    LOG_STATIC( Logger::ERR, "Failed to start TCP server, exiting...");
    delete socketServer;
    SslConnection::destroy();
    Logger::destroy();
    return 1;
  }

  // never reached

  return 0;
}