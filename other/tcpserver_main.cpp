#include <cpp_utils/Logger.hpp>
#include <cpp_utils/Common.hpp>
#include <cpp_utils/TcpConnection.hpp>
#include <cpp_utils/SocketServer.hpp>

#include "EchoMessage.hpp"

#include <iostream>
#include <string>

#include <unistd.h> // sleep


int main(int argc, char* argv[] )
{
  if ( argc != 3 ) {
    std::cerr << "Usage: " << argv[0] <<  " <HOST> <PORT>" << std::endl;
    return 1;
  }

  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  EchoMessage msg;
  TcpConnection conn(argv[1], argv[2], &msg);
  SocketServer socketServer(&conn);

  if ( !socketServer.start() ) {
    LOG_STATIC( Logger::ERR, "Failed to start TCP server, exiting...");
    Logger::destroy();
    return 1;
  }

  // never reached
  sleep(1);

  socketServer.stop();
  Logger::destroy();
  return 0;
}