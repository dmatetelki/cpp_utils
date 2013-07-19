#include "Poll.hpp"

#include "Logger.hpp"
#include "Common.hpp"


#include <sys/types.h>
#include <sys/socket.h>



Poll::Poll( StreamConnection  *connection,
            const nfds_t       maxClient,
            const int          timeOut )
  : m_timeOut(timeOut)
  , m_connection(connection)
  , m_polling(false)
  , m_connections()
  , m_maxclients(maxClient)
  , m_fds(0)
  , m_num_of_fds(0)
{
  TRACE;
  m_fds = new pollfd[m_maxclients+1]; // plus the server socket
  addFd( m_connection->getSocket(), POLLIN | POLLPRI );
}


Poll::~Poll()
{
  TRACE;
  delete[] m_fds;
}


void Poll::startPolling()
{
  TRACE;

  m_polling = true;
  while ( m_polling ) {

    int ret = poll( m_fds , m_maxclients, m_timeOut);

    if ( ret == -1 ) {
        LOG( Logger::ERR, errnoToString("ERROR polling. ").c_str() );
        /// @todo reconnect at client case?
        return;
    }

    if ( ret != 0 ) {  // not timeout
      for ( nfds_t i = 0; i < m_num_of_fds; ++i )
        if ( m_fds[i].revents != 0 )
          m_fds[i].fd == m_connection->getSocket() ?
              acceptClient() :
              handleClient(m_fds[i].fd);
    }

    removeTimeoutedConnections();

  } // while
}


void Poll::stopPolling()
{
  TRACE;
  m_polling = false;
}


bool Poll::isPolling() const
{
  TRACE;
  return m_polling;
}


void Poll::acceptClient()
{
  TRACE;

  int client_socket = -1;
  if ( !m_connection->accept( client_socket ) )
    return;

  StreamConnection *streamConnection = dynamic_cast<StreamConnection*>(
                                          m_connection->clone(client_socket));

  LOG_BEGIN(Logger::INFO)
    LOG_PROP("host", streamConnection->getHost())
    LOG_PROP("port", streamConnection->getPort())
    LOG_PROP("socket", client_socket)
  LOG_END("New client connected.");

  m_connections[client_socket] = streamConnection;
  addFd( client_socket, POLLIN | POLLPRI );
}


void Poll::handleClient( const int socket )
{
  TRACE;

  ConnectionMap::iterator it = m_connections.find(socket);
  if (it == m_connections.end()) {
    LOG_BEGIN(Logger::ERR)
      LOG_SPROP(socket)
    LOG_END("Socket not found in map.");
    return;
  }

  if (!it->second->receive())
    removeConnection(socket, it);
}


void Poll::removeTimeoutedConnections()
{
//   TRACE;
//
//   if (m_connections.empty())
//     return;
//
//   ConnectionMap::iterator it;
//   for (it = m_connections.begin(); it != m_connections.end(); )
//
//     /// @bug pull up closed() from TcpConnection to StreamConnection?
//     if (it->second->closed()) {
//       it = removeConnection(it->second->getSocket(), it);
//     } else {
//       ++it;
//     }
}


Poll::ConnectionMap::iterator Poll::removeConnection(int socket, std::map< int, StreamConnection* >::iterator it)
{
  TRACE;

  ConnectionMap::iterator next = it;
  next++;

  removeFd(socket);
  delete it->second;
  m_connections.erase(it);

  return next;
}


bool Poll::addFd( const int socket, const short events )
{
  TRACE;
  LOG_BEGIN(Logger::DEBUG)
    LOG_SPROP(socket)
  LOG_END("Adding socket.");

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
  LOG_BEGIN(Logger::DEBUG)
    LOG_SPROP(socket)
  LOG_END("Removing socket.");

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
