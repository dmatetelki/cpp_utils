#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include "SocketConnection.hpp"
#include "Poll.hpp"


class SocketServer
{
public:

  SocketServer ( SocketConnection   *connection,
                 const int           maxClients = 5,
                 const int           maxPendingQueueLen = 10 );

  virtual ~SocketServer();

  bool start();
  void stop();


private:

  SocketServer(const SocketServer&);
  SocketServer& operator=(const SocketServer&);

  SocketConnection  *m_connection;
  Poll               m_poll;
  const int          m_maxPendingQueueLen;
};

#endif // SOCKET_SERVER_HPP
