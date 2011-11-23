#include "SocketServer.hpp"

#include "Logger.hpp"

SocketServer::SocketServer ( SocketConnection   *connection,
                             const int           maxClients,
                             const int           maxPendingQueueLen )
  : m_connection(connection)
  , m_poll( m_connection, maxClients)
  , m_maxPendingQueueLen(maxPendingQueueLen)
{
  TRACE;
}


SocketServer::~SocketServer()
{
  TRACE;
}


bool SocketServer::start()
{
  TRACE;

  if ( !m_connection->bindToHost() )
    return false;

  if ( m_connection->listen( m_maxPendingQueueLen ) == -1 ) {
    return false;
  }

  m_poll.startPolling();
  return true;
}


void SocketServer::stop()
{
  TRACE;
  m_poll.stopPolling();
  m_connection->closeConnection();
}
