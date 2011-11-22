// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "Logger.hpp"

#include "TcpClient.hpp"
#include "Connection.hpp"
#include "Message.hpp"


#include <iostream>
#include <string>

#include <time.h> // nanosleep




class SimpleMessage : public Message
{
public:

  SimpleMessage( void *msgParam = 0)
    : Message(msgParam)
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

  Message* clone()
  {
    TRACE;
    return new SimpleMessage(m_param);
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
    std::cerr << "Usage: " << argv[0] <<  " <HOST> <PORT> <MSG>" << std::endl;
    return 1;
  }

  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);

  bool finished = false;

  SimpleMessage msg(&finished);

  TcpClient tcpclient( argv[1],
                       argv[2],
                       &msg);

  if ( !tcpclient.connect() ) {
    LOG( Logger::ERR, "Couldn't connect to server, exiting..." );
    Logger::destroy();
    return 1;
  }

  // wait for thread creation
  sleep(1);

  // send message to server
  std::string msg1(argv[3]);
  if ( !tcpclient.send( msg1.c_str(), msg1.length()) ) {
    LOG( Logger::ERR, "Couldn't send message to server, exiting..." );
    Logger::destroy();
    return 1;
  }

  // wait for the complate &handled reply
  struct timespec tm = {0,1000};
  while ( !finished && tcpclient.isPolling() )
    nanosleep(&tm, &tm) ;

  tcpclient.disconnect();
  Logger::destroy();
  return 0;
}