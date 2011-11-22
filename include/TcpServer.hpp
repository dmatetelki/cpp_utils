#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "Connection.hpp"
#include "Poll.hpp"
#include "Message.hpp"


#include <string>

class TcpServer
{
public:

  TcpServer ( const std::string   host,
              const std::string   port,
              Message            *message,
              const int           maxClients = 5,
              const int           maxPendingQueueLen = 10 );

  virtual ~TcpServer();

  bool start();
  void stop();


private:

  TcpServer(const TcpServer&);
  TcpServer& operator=(const TcpServer&);

  Connection      m_connection;
  Poll            m_poll;
  const int       m_maxPendingQueueLen;
};

#endif // TCP_SERVER_HPP
