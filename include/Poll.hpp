#ifndef POLL_HPP
#define POLL_HPP

#include "Logger.hpp"

#include "Connection.hpp"

#include <poll.h>
#include <map>



template <typename T>
class Poll
{
public:

  Poll( Connection<T>  *connection,
        const nfds_t    maxClient = 10 )
    : m_connection(connection)
    , m_polling(false)
    , m_connectionPool()
    , m_maxclients(maxClient)
    , m_fds(0)
    , m_num_of_fds(0)
  {
    TRACE;
    m_fds = new pollfd[m_maxclients];
    addFd( m_connection->getSocket(), POLLIN | POLLPRI );
  }

  virtual ~Poll()
  {
    TRACE;
    delete[] m_fds;
  }

  void startPolling()
  {
    TRACE;

    m_polling = true;
    struct timespec tm = {0,1000};

    while ( m_polling ) {

      nanosleep(&tm, &tm) ;
      int ret = poll( m_fds , m_maxclients, 1000);

      if ( ret == -1 ) {
          LOG( Logger::ERR, errnoToString("ERROR polling. ").c_str() );
          /// @todo reconnect
          return;
      }

      if ( ret == 0 )  // timeout
        continue;

      for ( nfds_t i = 0; i < m_num_of_fds; ++i )
        if ( m_fds[i].revents != 0 )
          m_fds[i].fd == m_connection->getSocket() ?
              acceptClient() :
              handleClient(m_fds[i].fd);

    } // while
  }

  void stopPolling()
  {
    TRACE;
    m_polling = false;
  }


protected:

  virtual void acceptClient()
  {
    TRACE;

    sockaddr clientAddr;
    socklen_t clientAddrLen;
    int client_socket = accept( m_connection->getSocket(),
                                &clientAddr, &clientAddrLen ) ;

    if ( client_socket == -1 ) {
      LOG( Logger::ERR, errnoToString("ERROR accepting. ").c_str() );
      return;
    }

    Connection<T> *connection = new Connection<T>(client_socket);

    LOG( Logger::INFO, std::string("New client connected: ").
                            append(connection->getHost()).append(":").
                            append(connection->getPort()).c_str() );

    m_connectionPool[client_socket] = new Connection<T>(client_socket);
    addFd( client_socket, POLLIN | POLLPRI );
  }

  virtual void handleClient( const int socket )
  {
    TRACE;

    typename ConnectionPool::iterator it = m_connectionPool.find(socket);

    if ( it == m_connectionPool.end() || !it->second->receive() ) {
      delete it->second;
      m_connectionPool.erase(it);
      removeFd(socket);
    }
  }


private:

  Poll(const Poll&);
  Poll& operator=(const Poll&);

  bool addFd( const int socket, const short events )
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

  bool removeFd( const int socket )
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


  typedef typename std::map< int, Connection<T>* > ConnectionPool;

  Connection<T>  *m_connection;
  bool            m_polling;
  ConnectionPool  m_connectionPool;

  nfds_t          m_maxclients;
  pollfd         *m_fds;
  nfds_t          m_num_of_fds;

};

#endif // POLL_HPP
