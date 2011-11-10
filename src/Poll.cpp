#include "Poll.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "Socket.hpp"

#include <stdlib.h> // malloc, free


Poll::Poll ( int &socket, const nfds_t maxClient )
  : m_polling(false)
  , m_pollSocket(socket)
  , m_maxclients(maxClient)
  , m_fds(0)
  , m_num_of_fds(0)
{
  TRACE;

  m_fds = (pollfd*) malloc (sizeof(struct pollfd)*m_maxclients);
}


Poll::~Poll()
{
  TRACE;

  free(m_fds);
}


void Poll::setOwnSocket ( const int socket )
{
  TRACE;

  addFd(socket, POLLIN | POLLPRI);
}


void Poll::startPolling()
{
  TRACE;

  m_polling = true;
  struct timespec tm = {0,10000};

  while ( m_polling ) {

    nanosleep(&tm, &tm) ;
    int ret = poll( m_fds , m_maxclients, 1000);

    if ( ret == -1 ) {
        LOG( Logger::ERR, errnoToString("ERROR polling. ").c_str() );
        /// @todo shall we handle this?
        return;
    }

    if ( ret == 0 )  // timeout
      continue;

    for ( nfds_t i = 0; i < m_num_of_fds; ++i )
      if ( m_fds[i].revents != 0 )
        m_fds[i].fd == m_pollSocket ?
            acceptClient() :
            handleClient(m_fds[i].fd);


  } // while
}


void Poll::stopPolling()
{
  TRACE;

  m_polling = false;
}


void Poll::acceptClient()
{
  TRACE;

  sockaddr clientAddr;
  socklen_t clientAddrLen;
  int client_socket = accept( m_pollSocket, &clientAddr, &clientAddrLen ) ;

  if ( client_socket == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR accepting. ").c_str() );
  } else {

    /// @bug does not works every time
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


void Poll::handleClient( const int socket )
{
  TRACE;

  if ( !receive( socket ) ) {
    removeFd( socket );
  }
}


bool Poll::addFd( const int socket, short events )
{
  TRACE;
  LOG( Logger::DEBUG, std::string("Adding socket: ").
                        append(TToStr(socket)).c_str() );

  if (m_num_of_fds >= m_maxclients )
    return false;

  m_fds[m_num_of_fds].fd = socket;
  m_fds[m_num_of_fds].events = events;
  m_fds[m_num_of_fds].revents = 0;
  m_num_of_fds++;

  return true;
}


bool Poll::removeFd( const int socket )
{
  TRACE;
  LOG( Logger::DEBUG, std::string("Removing socket: ").
                        append(TToStr(socket)).c_str() );

  unsigned int i = 0 ;
  while (i < m_maxclients && m_fds[i].fd != socket )
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