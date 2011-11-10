#include "TcpServer.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <poll.h>
#include <stdlib.h>

TcpServer::TcpServer( const std::string host,
                      const std::string port,
                      const int maxClients )
  : Socket(AF_INET, SOCK_STREAM)
  , Poll(m_socket, maxClients)
  , m_host(host)
  , m_port(port)
{
  TRACE;
}


TcpServer::~TcpServer()
{
  TRACE;
}


bool TcpServer::start()
{
  TRACE;

  if ( !openSocket() )
    return false;

  if ( !bindToHost(m_host, m_port) )
    return false;

  if ( listen(m_socket, 64) == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR listening. ").c_str() );
    return false;
  }

  addFd( m_socket, POLLIN | POLLPRI ) ;
  startPolling();

  return true;
}


void TcpServer::stop()
{
  TRACE;

  stopPolling();
  closeSocket();
}


bool TcpServer::receive(const int clientSocket)
{
  TRACE;

  char buffer[256];
  int len = recv( clientSocket, buffer , 256, 0) ;

  if (len == -1) {
    LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
    return false;
  }

  if (len == 0) {
    LOG( Logger::DEBUG, "Connection closed by peer." );
    return false;
  }

  std::string msg(buffer, len);
  msgArrived(clientSocket, msg);

  return true;
}
