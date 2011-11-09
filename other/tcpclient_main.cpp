// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "TcpClient.hpp"

#include "Logger.hpp"

#include <iostream>
#include <string>

int main( int argc, char * argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  TcpClient tcpclient("localhost", "4455");

  tcpclient.connect();

  tcpclient.send("madao");

  std::string reply;
  tcpclient.receive(reply);
  std::cout << "got reply \"" << reply << "\"" << std::endl;

  tcpclient.disconnect();



  Logger::destroy();
  return 0;
}