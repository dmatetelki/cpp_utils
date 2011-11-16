#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "Logger.hpp"

#include "Connection.hpp"
#include "Poll.hpp"



#include <string>

template <typename T>
class TcpServer
{
public:

  TcpServer ( const std::string host,
              const std::string port,
              const int maxClients = 5,
              const int maxPendingQueueLen = 10 )
    : m_connection(host, port)
    , m_poll( &m_connection, maxClients)
    , m_maxPendingQueueLen(maxPendingQueueLen)
  {
    TRACE;
  }

  virtual ~TcpServer()
  {
    TRACE;
  }

  bool start()
  {
    TRACE;

    if ( !m_connection.bindToHost() )
      return false;

    if ( m_connection.listen( m_maxPendingQueueLen ) == -1 ) {
      return false;
    }

    m_poll.startPolling();
    return true;
  }

  void stop()
  {
    TRACE;
    m_poll.stopPolling();
    m_connection.closeConnection();
  }


private:

  TcpServer(const TcpServer&);
  TcpServer& operator=(const TcpServer&);

  Connection<T>   m_connection;
  Poll<T>         m_poll;
  const int       m_maxPendingQueueLen;
};

#endif // TCP_SERVER_HPP
