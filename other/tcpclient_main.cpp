// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "Logger.hpp"

#include "TcpClient.hpp"
#include "MessageBuilder.hpp"

#include <iostream>
#include <string>


class DummyBuilder : public MessageBuilder
{
private:

  void messageBuilt( const unsigned char* message,
                     const int length )
  {
    TRACE;

    std::string reply((char *)message, length);
    LOG( Logger::INFO, std::string("Got reply from server: ").
                        append(reply).c_str() );
  }

};


class PrinterTcpClient : public TcpClient
{
public:
  PrinterTcpClient ( const std::string host,
                     const std::string port,
                     MessageBuilder *builder
                   )
    : TcpClient(host, port, builder)
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

  MessageBuilder *builder = new DummyBuilder;

  PrinterTcpClient tcpclient("localhost", "4455", builder);

  tcpclient.connect();

  sleep(2);
  std::string msg1("madao");
  tcpclient.send( msg1.c_str(), msg1.length());
  sleep(2);
  std::string msg2("this message is long. Cannot fit into one buffer");
  tcpclient.send( msg2.c_str(), msg2.length());
  sleep(2);

//   std::string reply;
//   tcpclient.receive(reply);
//   std::cout << "got reply \"" << reply << "\"" << std::endl;

  tcpclient.disconnect();


  delete builder;
  Logger::destroy();
  return 0;
}