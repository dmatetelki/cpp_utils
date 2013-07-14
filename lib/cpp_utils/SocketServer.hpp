#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include "StreamConnection.hpp"
#include "Poll.hpp"


class SocketServer
{
public:

  SocketServer ( StreamConnection  *connection,
                 const int          maxClients = 5,
                 const int          maxPendingQueueLen = 10 );

  virtual ~SocketServer();

  bool start();
  void stop();


private:

  SocketServer(const SocketServer&);
  SocketServer& operator=(const SocketServer&);

  StreamConnection  *m_connection;
  Poll               m_poll;
  const int          m_maxPendingQueueLen;
};

#endif // SOCKET_SERVER_HPP
