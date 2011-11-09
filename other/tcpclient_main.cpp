// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "TcpClient.hpp"

#include "Logger.hpp"

#include <iostream>
#include <string>


class PrinterTcpClient : public TcpClient
{
public:
  PrinterTcpClient ( const std::string host,
                     const std::string port )
    : TcpClient(host, port)
  {
    TRACE;
  }

private:

  void msgArrived( const std::string msg)
  {
    TRACE;
    LOG( Logger::DEBUG, std::string("Got msg: ").append(msg).c_str() );
  }

  void onDisconnect()
  {
    TRACE;
    LOG( Logger::DEBUG, "What shall I do..." );
  }

};

int main( int argc, char * argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  PrinterTcpClient tcpclient("localhost", "4455");

  tcpclient.connect();

  sleep(2);
  tcpclient.send("madao");
  sleep(2);

//   std::string reply;
//   tcpclient.receive(reply);
//   std::cout << "got reply \"" << reply << "\"" << std::endl;

  tcpclient.disconnect();



  Logger::destroy();
  return 0;
}