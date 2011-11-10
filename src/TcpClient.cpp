#include "TcpClient.hpp"

#include "Logger.hpp"
#include "Common.hpp"


TcpClient::TcpClient( const std::string host,
                      const std::string port )
  : Socket(AF_INET, SOCK_STREAM)
  , m_host(host)
  , m_port(port)
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

  if ( !openSocket() )
    return false;

  if ( !connectToHost(m_host, m_port) )
    return false;

  m_watcher.setOwnSocket(m_socket);
  m_watcher.start();
  return true;
}


void TcpClient::disconnect()
{
  TRACE;

  closeSocket();

  if ( m_watcher.isRunning() ) {
    m_watcher.stopPolling();
    m_watcher.stop();
    m_watcher.join();
  }
}


bool TcpClient::send(const std::string msg)
{
  TRACE;

  ssize_t n = write(m_socket, msg.c_str(), msg.length());
  if (n == -1) {
    LOG( Logger::ERR, errnoToString("ERROR writing to socket. ").c_str() );
    return false;
  }

  return true;
}


TcpClient::WatcherThread::WatcherThread( TcpClient &data )
  : Poll(data.m_socket, 1)
  , m_tcpClient(data)
{
  TRACE;
}


void TcpClient::WatcherThread::acceptClient()
{
  TRACE;

  // not accepting anything
  receive( m_tcpClient.m_socket );
}


void TcpClient::WatcherThread::handleClient( const int fd )
{
  TRACE;

  LOG( Logger::DEBUG, "Server closed the connection." );
}


bool TcpClient::WatcherThread::receive( const int fd)
{
  TRACE;

  char buffer[14];
  int len = recv( fd, buffer , 14, 0) ;

  if (len == -1) {
    LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
    return false;
  }

  if (len == 0) {
    LOG( Logger::DEBUG, "Connection closed by peer." );
    return false;
  }

  std::string msg(buffer, len);
  m_tcpClient.msgArrived(msg);

  return true;
}


void* TcpClient::WatcherThread::run()
{
  TRACE;

  startPolling();
  return 0;
}
