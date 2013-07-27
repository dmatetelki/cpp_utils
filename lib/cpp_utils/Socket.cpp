#include "Socket.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "AddrInfo.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntop
#include <sys/select.h>

#include <unistd.h>

#include <string.h> // strerror
#include <errno.h> // errno


Socket::Socket(const int domain,
               const int type,
               const int protocol)
  : m_socket(-1)
  , m_domain(domain)
  , m_type(type)
  , m_protocol(protocol)
{
  TRACE;
}


Socket::Socket(const int socket)
  : m_socket(socket)
  , m_domain(-1)
  , m_type(-1)
  , m_protocol(-1)
{
  TRACE;

  /// @todo get domain type prot from socket
}


Socket::~Socket()
{
  TRACE;
}


bool Socket::createSocket()
{
  TRACE;

  m_socket = socket(m_domain, m_type, m_protocol);
  if ( m_socket == -1 ) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not create socket.");
    return false;
  }
  return true;
}


bool Socket::closeSocket()
{
  TRACE;

  if (close(m_socket) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not close socket.");
    return false;
  }

  m_socket = -1;
  return true;
}


bool Socket::connect(struct addrinfo *servinfo)
{
  TRACE;

  if (servinfo == 0)
    return false;

  if (::connect(m_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not connect to peer.");
    return false;
  }
  return true;
}


bool Socket::bind(struct addrinfo *servinfo )
{
  TRACE;

  if (servinfo == 0)
    return false;

  if (::bind(m_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not bind name to socket.");
    return false;
  }
  return true;
}


bool Socket::listen(const int maxPendingQueueLen)
{
  TRACE;

  if ( ::listen(m_socket, maxPendingQueueLen) == -1 ) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not listen on socket.");
    return false;
  }
  return true;
}


bool Socket::accept(int client_socket)
{
  TRACE;
  sockaddr clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);

  client_socket = ::accept( m_socket, &clientAddr, &clientAddrLen ) ;
  if ( client_socket == -1 ) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not accept connection on socket.");
    return false;
  }
  return true;
}


bool Socket::send ( const void *message, const int length )
{
  TRACE;

  if ( ::send(m_socket, message, length, MSG_NOSIGNAL) == -1 ) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not send message to socket.");
    return false;
  }
  return true;
}


bool Socket::receive( void *buffer, const int bufferLen, ssize_t *msgLen )
{
  TRACE;

  *msgLen = recv(m_socket, buffer, bufferLen, 0);
  if (*msgLen == -1) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message", strerror(errno))
    LOG_END("Could not read from socket.");
    return false;
  }
  return true;
}


void Socket::getPeerName( std::string &host,
                          std::string &port )
{
  TRACE;

  struct sockaddr_in  address ;
  memset(&address, 0, sizeof(address));
  socklen_t addressLength = sizeof(address) ;
  getpeername( m_socket, (struct sockaddr*)&address, &addressLength ) ;

  char tmp[INET_ADDRSTRLEN];
  host = inet_ntop(AF_INET, &address.sin_addr, tmp, INET_ADDRSTRLEN);
  port = TToStr(address.sin_port);
}


int Socket::getSocket() const
{
  TRACE;
  return m_socket;
}
