#include "TcpServer.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <poll.h>
#include <stdlib.h>

TcpServer::TcpServer( const std::string host,
                      const std::string port,
                      const int maxClients )
  : Socket(AF_INET, SOCK_STREAM)
  , m_host(host)
  , m_port(port)
  , m_maxclients(maxClients)
  , m_running(false)
  , m_fds(0)
  , m_num_of_fds(0)
  , m_addr()
  , m_addrLen(0)
{
  TRACE;
  m_fds = (pollfd*) malloc (sizeof(struct pollfd)*m_maxclients);
}


TcpServer::~TcpServer()
{
  TRACE;
  free(m_fds);
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

  m_running = true;

  struct timespec tm = {0,1000};

  while ( m_running ) {

    nanosleep(&tm, &tm) ;
    int ret = poll( m_fds , m_maxclients, 1000);

    if ( ret == -1 ) {
        LOG( Logger::ERR, errnoToString("ERROR polling. ").c_str() );
        return false;
    }

    if ( ret == 0 )  // timeout
      continue;

    for ( nfds_t i = 0; i < m_num_of_fds; ++i ) {
      if ( m_fds[i].revents != 0 ) {

        if ( m_fds[i].fd == m_socket ) {

          sockaddr clientAddr;
          socklen_t clientAddrLen;
          int client_socket = accept( m_socket, &clientAddr, &clientAddrLen ) ;

          if ( client_socket == -1 ) {
            LOG( Logger::ERR, errnoToString("ERROR accepting. ").c_str() );
          } else {

            std::string clientAddress, clientService;
            if ( Socket::convertNameInfo(&clientAddr, clientAddrLen,
                                         clientAddress, clientService ) ) {
              LOG( Logger::DEBUG, std::string("New client connected: ").
                                    append(clientAddress).append(":").
                                    append(clientService).c_str() );
            }
            addFd( client_socket, POLLIN | POLLPRI );
          }
        }
        else {
          if ( !receive( m_fds[i].fd ) ) {
            removeFd(m_fds[i].fd);
          }
        }
      }
    }

  }


  return true;
}


void TcpServer::stop()
{
  TRACE;

  m_running = false;

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


void TcpServer::addFd( int fd, short events )
{
  TRACE;

  if (m_num_of_fds < m_maxclients )
  {
    m_fds[m_num_of_fds].fd = fd ;
    m_fds[m_num_of_fds].events = events ;
    m_fds[m_num_of_fds].revents = 0 ;
    ++m_num_of_fds ;
  }
}

void TcpServer::removeFd( int fd )
{
  TRACE;

  unsigned int i = 0 ;
  while (i < m_maxclients && m_fds[i].fd != fd ) ++i ;

  if ( i != m_maxclients ) {
    for ( ; i < m_maxclients - 1; ++i )
      m_fds[i] = m_fds[i+1] ;

    m_fds[i].fd = 0 ;
    m_fds[i].events = 0 ;
    m_fds[i].revents = 0 ;
    --m_num_of_fds ;
  }
}