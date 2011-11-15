// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "Logger.hpp"

#include "TcpClient.hpp"
#include "Message.hpp"

#include <iostream>
#include <string>

class SimpleMessage : public Message
{
private:

  bool buildMessage( const unsigned char* messagePart,
                     const int length )
  {
    TRACE;
    m_buffer = std::string( (const char*) messagePart, length );
    onMessageReady();
  }

  void onMessageReady()
  {
    TRACE;
    LOG( Logger::INFO, std::string("Got reply from server: ").
                        append(m_buffer).c_str() );
  }

protected:

  int getExpectedLength()
  {
    TRACE;
    return 0;
  }
};


int main( int argc, char * argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  TcpClient<SimpleMessage> tcpclient("localhost", "4455");

  tcpclient.connect();

  sleep(2);
  std::string msg1("madao");
  tcpclient.send( msg1.c_str(), msg1.length());
  sleep(2);
  std::string msg2("this message is long. Cannot fit into one buffer");
  tcpclient.send( msg2.c_str(), msg2.length());
  sleep(2);


  tcpclient.disconnect();

  Logger::destroy();
  return 0;
}