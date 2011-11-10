#include "TcpClient.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntop

#include <poll.h>
#include <time.h>

TcpClient::TcpClient( const std::string host,
                      const std::string port )
  : Socket(AF_INET, SOCK_STREAM)
  , m_host(host)
  , m_port(port)
  , m_connected(false)
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

  m_connected = true;

  m_watcher.start();
  return true;
}


void TcpClient::disconnect()
{
  TRACE;

  closeSocket();
  m_connected = false;

  if ( m_watcher.isRunning() ) {
    m_watcher.stop();
    m_watcher.join();
  }
}


bool TcpClient::send(const std::string msg)
{
  TRACE;

  if ( !m_connected )
    return false;

  ssize_t n = write(m_socket, msg.c_str(), msg.length());
  if (n == -1) {
    LOG( Logger::ERR, errnoToString("ERROR writing to socket. ").c_str() );
    return false;
  }

  return true;
}


TcpClient::WatcherThread::WatcherThread( TcpClient &data )
  : m_tcpClient(data)
{
  TRACE;
}


void* TcpClient::WatcherThread::run()
{
  TRACE;

  struct timespec tm = {0,1000};
  while ( m_isRunning ) {

    nanosleep(&tm, &tm) ;
    if ( m_tcpClient.m_connected ) {

      pollfd fds[1];
      fds[0].fd       = m_tcpClient.m_socket ;
      fds[0].events   = POLLIN | POLLPRI ;
      fds[0].revents  = 0 ;

      int ret = poll( fds , 1, 1000) ;
      if ( ret == -1 ) {
        LOG( Logger::ERR, errnoToString("ERROR polling. ").c_str() );
        m_tcpClient.m_connected = false;
        m_tcpClient.onDisconnect();
      }

      if ( ret != 0 && !receive() ) {
          m_tcpClient.m_connected = false;
          m_tcpClient.onDisconnect();
      }

    }
  }

  return 0;
}


bool TcpClient::WatcherThread::receive()
{
  TRACE;

  char buffer[256];
  int len = recv( m_tcpClient.m_socket, buffer , 256, 0) ;

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