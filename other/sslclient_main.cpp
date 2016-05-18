#include <cpp_utils/Logger.hpp>
#include <cpp_utils/SslConnection.hpp>
#include <cpp_utils/SocketClient.hpp>

#include "PrintMessage.hpp"

#include <iostream>
#include <string>


int main(int argc, char* argv[] )
{
  if ( argc != 4 ) {
    std::cerr << "Usage: " << argv[0] <<  " <HOST> <PORT> <MSG>" << std::endl;
    return 1;
  }

  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);
  SslConnection::init();

  PrintMessage msg;
  SslConnection conn(argv[1], argv[2], &msg);
  conn.initClientContext();
  SocketClient socketClient(&conn);
  if ( !socketClient.connect() ) {
    LOG_STATIC( Logger::ERR, "Couldn't connect to server, exiting..." );
    SslConnection::destroy();
    Logger::destroy();
    return 1;
  }

  // send message to server
  std::string msg1(argv[3]);
  if ( !socketClient.send( msg1.c_str(), msg1.length()) ) {
    LOG_STATIC( Logger::ERR, "Couldn't send message to server, exiting..." );
    SslConnection::destroy();
    Logger::destroy();
    return 1;
  }
  msg.waitForReady();

  socketClient.disconnect();
  SslConnection::destroy();
  Logger::destroy();
  return 0;
}