#include "TcpClient.hpp"

#include "Logger.hpp"


// PollerThread

TcpClient::PollerThread::PollerThread( TcpClient* data )
  : Poll( &(data->m_connection) )
  , m_tcpClient(data)
{
  TRACE;
}


void TcpClient::PollerThread::stopPoller()
{
  TRACE;
  stopPolling();
  stop();
}


void TcpClient::PollerThread::acceptClient()
{
  TRACE;

  m_tcpClient->m_connection.receive();
  stopPolling();
}


void TcpClient::PollerThread::handleClient( const int )
{
  TRACE;
  LOG( Logger::DEBUG, "Server closed the connection." );
  stopPolling();
}


void* TcpClient::PollerThread::run()
{
  TRACE;
  startPolling();
  return 0;
}


// TcpClient

TcpClient::TcpClient (  const std::string   host,
                        const std::string   port,
                        Message            *message )
  : m_connection (host, port, message)
  , m_watcher(this)
{
  TRACE;

  message->setConnection(&m_connection);
}


TcpClient::~TcpClient()
{
  TRACE;
  disconnect();
}


bool TcpClient::connect()
{
  TRACE;

  if ( !m_connection.connectToHost() )
    return false;

  m_watcher.start();
  return true;
}


void TcpClient::disconnect()
{
  TRACE;

  if ( m_watcher.isRunning() ) {
    m_watcher.stopPoller();
    m_watcher.join();
  }

  m_connection.closeConnection();
}


bool TcpClient::send( const void* msg, const size_t msgLen )
{
  TRACE;
  return m_connection.send(msg, msgLen);
}


bool TcpClient::isPolling() const
{
  TRACE;
  return m_watcher.isPolling();
}
