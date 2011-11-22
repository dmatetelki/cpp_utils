#include "TcpServer.hpp"

#include "Logger.hpp"

TcpServer::TcpServer ( const std::string   host,
                       const std::string   port,
                       Message            *message,
                       const int           maxClients,
                       const int           maxPendingQueueLen )
  : m_connection(host, port, message)
  , m_poll( &m_connection, maxClients)
  , m_maxPendingQueueLen(maxPendingQueueLen)
{
  TRACE;

  message->setConnection(&m_connection);
}


TcpServer::~TcpServer()
{
  TRACE;
}


bool TcpServer::start()
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


void TcpServer::stop()
{
  TRACE;
  m_poll.stopPolling();
  m_connection.closeConnection();
}
