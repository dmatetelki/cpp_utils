#include "TcpClient.hpp"

#include "Logger.hpp"
#include "Common.hpp"


TcpClient::TcpClient( const std::string host,
                      const std::string port,
                      MessageBuilder *builder )
  : m_connection (host, port, builder)
  , m_watcher(*this)
{
  TRACE;
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

  m_watcher.setOwnSocket(m_connection.getSocket());
  m_watcher.start();
  return true;
}


void TcpClient::disconnect()
{
  TRACE;

  m_connection.closeConnection();

  if ( m_watcher.isRunning() ) {
    m_watcher.stopPolling();
    m_watcher.stop();
    m_watcher.join();
  }
}


bool TcpClient::send( const void* message, const int length )
{
  TRACE;

  return m_connection.sendMessage(message, length);
}



// WatcherThread

TcpClient::WatcherThread::WatcherThread( TcpClient &data )
  : Poll(data.m_connection.getSocket())
  , m_tcpClient(data)
{
  TRACE;
}


void TcpClient::WatcherThread::acceptClient()
{
  TRACE;

  // not accepting anything
//   receive( m_tcpClient.m_socket );
}


void TcpClient::WatcherThread::handleClient( const int fd )
{
  TRACE;

  LOG( Logger::DEBUG, "Server closed the connection." );
  stopPolling();
}


// bool TcpClient::WatcherThread::receive( const int fd)
// {
//   TRACE;

//   char buffer[14];
//   int len = recv( fd, buffer , 14, 0) ;
//
//   if (len == -1) {
//     LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
//     return false;
//   }
//
//   if (len == 0) {
//     LOG( Logger::DEBUG, "Connection closed by peer." );
//     stopPolling();
//     return false;
//   }
//
//   std::string msg(buffer, len);
//   m_tcpClient.msgArrived(msg);
/*
  return true;
}*/


void* TcpClient::WatcherThread::run()
{
  TRACE;

  startPolling();
  return 0;
}
