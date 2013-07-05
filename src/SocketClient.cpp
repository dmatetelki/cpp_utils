#include "SocketClient.hpp"

#include "Logger.hpp"


// PollerThread

SocketClient::PollerThread::PollerThread( SocketClient* data )
  : Poll(data->m_connection)
  , m_tcpClient(data)
{
  TRACE;
}


void SocketClient::PollerThread::stopPoller()
{
  TRACE;
  stopPolling();
  stop();
}


void SocketClient::PollerThread::acceptClient()
{
  TRACE;

  m_tcpClient->m_connection->receive();
  stopPolling();
}


void SocketClient::PollerThread::handleClient( const int )
{
  TRACE;
  LOG( Logger::DEBUG, "Server closed the connection." );
  stopPolling();
}


void SocketClient::PollerThread::removeTimeoutedConnections()
{
}


void* SocketClient::PollerThread::run()
{
  TRACE;
  startPolling();
  return 0;
}


// SocketClient

SocketClient::SocketClient (StreamConnection *connection )
  : m_connection (connection)
  , m_watcher(this)
{
  TRACE;
}


SocketClient::~SocketClient()
{
  TRACE;
  disconnect();
}


bool SocketClient::connect()
{
  TRACE;

  if ( !m_connection->connect() )
    return false;

  m_watcher.start();
  return true;
}


void SocketClient::disconnect()
{
  TRACE;

  if ( m_watcher.isRunning() ) {
    m_watcher.stopPoller();
    m_watcher.join();
  }

  m_connection->disconnect();
}


bool SocketClient::send( const void* msg, const size_t msgLen )
{
  TRACE;
  return m_connection->send(msg, msgLen);
}


bool SocketClient::isPolling() const
{
  TRACE;
  return m_watcher.isPolling();
}
