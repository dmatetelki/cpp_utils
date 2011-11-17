// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "Logger.hpp"

#include "TcpClient.hpp"
#include "Connection.hpp"
#include "Message.hpp"


#include <iostream>
#include <string>

#include <time.h> // nanosleep




class SimpleMessage : public Message<SimpleMessage>
{
public:

  SimpleMessage( Connection<SimpleMessage>  *connection,
                 void                       *msgParam = 0)
    : Message<SimpleMessage>(connection, msgParam)
  {
    TRACE;
  }

  bool buildMessage( const void   *msgPart,
                     const size_t  msgLen )
  {
    TRACE;
    m_buffer = std::string( (const char*) msgPart, msgLen );
    onMessageReady();
    return true;
  }

  void onMessageReady()
  {
    TRACE;

    LOG( Logger::INFO, std::string("Got reply from server: ").
                        append(m_buffer).c_str() );

    *( static_cast<bool*>(m_param) ) = true;
  }

protected:

  size_t getExpectedLength()
  {
    TRACE;
    return 0;
  }
};


int main(int argc, char* argv[] )
{
  if ( argc != 4 ) {
    std::cerr << "Usage: client <HOST> <PORT> <MSG>" << std::endl;
    return 1;
  }

  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::DEBUG);

  bool finished = false;

  TcpClient<SimpleMessage> tcpclient(argv[1], argv[2], &finished);

  tcpclient.connect();
  sleep(1); // wait for thread creation

  std::string msg1(argv[3]);
  tcpclient.send( msg1.c_str(), msg1.length());

  // wait for the complate &handled reply
  struct timespec tm = {0,1000};
  while ( !finished )
    nanosleep(&tm, &tm) ;

  tcpclient.disconnect();

  Logger::destroy();
  return 0;
}