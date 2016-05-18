#include <cpp_utils/Logger.hpp>
#include <cpp_utils/Common.hpp>
#include <cpp_utils/TcpConnection.hpp>
#include <cpp_utils/SocketClient.hpp>

#include "PrintMessage.hpp"

#include <iostream>
#include <string>

#include <time.h> // nanosleep
#include <unistd.h> // sleep


int main(int argc, char* argv[] )
{
  if ( argc != 4 ) {
    std::cerr << "Usage: " << argv[0] <<  " <HOST> <PORT> <MSG>" << std::endl;
    return 1;
  }


  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  bool finished = false;
  PrintMessage msg(&finished);

  TcpConnection conn(argv[1], argv[2], &msg);
  SocketClient socketClient(&conn);

  if ( !socketClient.connect() ) {
    LOG_STATIC( Logger::ERR, "Couldn't connect to server, exiting..." );
    Logger::destroy();
    return 1;
  }

  // wait for thread creation
  sleep(1);

  // send message to server
  std::string msg1(argv[3]);
  if ( !socketClient.send( msg1.c_str(), msg1.length()) ) {
    LOG_STATIC( Logger::ERR, "Couldn't send message to server, exiting..." );
    Logger::destroy();
    return 1;
  }

  // wait for the complate &handled reply
  struct timespec tm = {0,1000};
  while ( !finished && socketClient.isPolling() )
    nanosleep(&tm, &tm) ;

  socketClient.disconnect();
  Logger::destroy();
  return 0;
}