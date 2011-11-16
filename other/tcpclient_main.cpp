// gpp tcpclient_main.cpp -o client -I../include ../src/Logger.cpp ../src/TcpClient.cpp

#include "Logger.hpp"

#include "TcpClient.hpp"
#include "Connection.hpp"
#include "Message.hpp"


#include <iostream>
#include <string>


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
  }

protected:

  size_t getExpectedLength()
  {
    TRACE;
    return 0;
  }
};


int main()
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::DEBUG);

  int *a = new int;
  *a=2;

  TcpClient<SimpleMessage> tcpclient("127.0.0.1", "4455");

  tcpclient.connect();

  sleep(2);
  std::string msg1("madao");
  tcpclient.send( msg1.c_str(), msg1.length());
  sleep(2);
//   std::string msg2("this message is long. Cannot fit into one buffer");
//   tcpclient.send( msg2.c_str(), msg2.length());
//   sleep(2);


  tcpclient.disconnect();

  delete a;

  Logger::destroy();
  return 0;
}