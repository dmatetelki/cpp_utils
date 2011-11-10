#include "Poll.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "Socket.hpp"

#include <sys/types.h>
#include <sys/socket.h>


#include <stdlib.h>

Poll::Poll ( const int socket, const nfds_t maxClient )
  : m_socket(socket)
  , m_maxclients()
  , m_fds(0)
  , m_num_of_fds(0)
  , m_polling(false)
{
  TRACE;

  m_fds = (pollfd*) malloc (sizeof(struct pollfd)*m_maxclients);
}


Poll::~Poll()
{
  TRACE;

  free(m_fds);
}


void Poll::startPolling()
{
  TRACE;

  struct timespec tm = {0,1000};

  while ( m_polling ) {

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
            acceptClient(m_fds[i].fd);
        }
        else {
          handleClient(m_fds[i].fd);
        }
      }
    }
  }
}


void Poll::stopPolling()
{
  TRACE;

  m_polling = false;
}


bool Poll::acceptClient()
{
  TRACE;

  sockaddr clientAddr;
  socklen_t clientAddrLen;
  int client_socket = accept( m_socket, &clientAddr, &clientAddrLen ) ;

  if ( client_socket == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR accepting. ").c_str() );
  } else {

    /// @bug always "bas family" errors
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


void Poll::handleClient( const int fd )
{
  TRACE;

  if ( !receive( fd ) ) {
    removeFd( fd );
  }
}


bool Poll::receive( const int fd)
{
  TRACE;

}


bool Poll::addFd( const int fd, short events )
{
  TRACE;

  if (m_num_of_fds >= m_maxclients )
    return false;

  m_fds[m_num_of_fds].fd = fd;
  m_fds[m_num_of_fds].events = events;
  m_fds[m_num_of_fds].revents = 0;
  m_num_of_fds++;

  return true;
}


bool Poll::removeFd( const int fd )
{
  TRACE;

  unsigned int i = 0 ;
  while (i < m_maxclients && m_fds[i].fd != fd )
    i++;

  if ( i == m_maxclients )
    return false;

  for ( ; i < m_maxclients - 1; ++i )
    m_fds[i] = m_fds[i+1] ;

  m_fds[i].fd = 0 ;
  m_fds[i].events = 0 ;
  m_fds[i].revents = 0 ;
  m_num_of_fds--;

  return true;
}