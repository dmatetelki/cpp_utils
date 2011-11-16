#include "Socket.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntop
#include <sys/select.h>


Socket::Socket(const int domain,
               const int type,
               const int protocol)
  : m_socket(-1)
  , m_domain(domain)
  , m_type(type)
  , m_protocol(protocol)
  , m_addr()
  , m_addrLen(0)
{
  TRACE;
}


Socket::Socket(const int socket)
  : m_socket(socket)
  , m_domain(-1)
  , m_type(-1)
  , m_protocol(-1)
  , m_addr()
  , m_addrLen(0)
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
    LOG( Logger::ERR,  errnoToString("ERROR opening socket. ").c_str() );
    return false;
  }

  return true;
}


void Socket::closeSocket()
{
  TRACE;

  /// @note are the return values of shutdown and close meaningful?
  shutdown(m_socket, SHUT_RDWR);
  close(m_socket);
  m_socket = -1;
}


bool Socket::connectToHost( const std::string host,
                            const std::string port )
{
  TRACE;

  struct addrinfo *results(0);
  if ( !Socket::getHostInfo(host, port, &results) )
    return false;

  Socket::printHostDetails(results);

  if ( !connectToFirstAddress(results) )
    return false;

  freeaddrinfo(results);
  return true;
}


bool Socket::bindToHost( const std::string host,
                         const std::string port )
{
  TRACE;

  struct addrinfo *results(0);
  if ( !Socket::getHostInfo(host, port, &results) )
    return false;

  Socket::printHostDetails(results);

  if ( !bindToFirstAddress(results) )
    return false;

  freeaddrinfo(results);
  return true;
}


bool Socket::listen ( const int maxPendingQueueLen )
{
  TRACE;

  if ( ::listen(m_socket, maxPendingQueueLen) == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR listening. ").c_str() );
    return false;
  }
  return true;
}


bool Socket::send ( const void *message, const int length )
{
  TRACE;

  if ( ::send(m_socket, message, length, MSG_NOSIGNAL) == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR sending. ").c_str() );
    return false;
  }

  return true;
}


int Socket::getSocket() const
{
  TRACE;
  return m_socket;
}


bool Socket::connectToFirstAddress(struct addrinfo *servinfo)
{
  TRACE;

  for ( struct addrinfo *it = servinfo; it != 0; it = it->ai_next)
    if (::connect(m_socket, it->ai_addr, it->ai_addrlen) != -1) {
      std::string address, service;
      if ( convertNameInfo( it->ai_addr, it->ai_addrlen, address, service) ) {
        LOG( Logger::DEBUG, std::string("Connected to ").
                              append(address).append(":").
                              append(service).c_str() );
      }
      return true;
    }

  LOG( Logger::ERR, "Could not connect to host, connection refused." );
  return false;
}


bool Socket::bindToFirstAddress(struct addrinfo *servinfo )
{
  TRACE;

  for ( struct addrinfo *it = servinfo; it != 0; it = it->ai_next)
    if (bind(m_socket, it->ai_addr, it->ai_addrlen) == 0) {
      memcpy(&m_addr, it->ai_addr, it->ai_addrlen);
      m_addrLen = it->ai_addrlen;

      std::string address, service;
      if ( Socket::convertNameInfo( &m_addr, m_addrLen, address, service) ) {
        LOG( Logger::DEBUG, std::string("Binded to ").
                              append(address).append(":").
                              append(service).c_str() );
      }
      return true;
    }

  LOG( Logger::ERR, "Could not bind to host. Address already in use." );

  return false;
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


bool Socket::getHostInfo( const std::string host,
                          const std::string port,
                          struct addrinfo **servinfo)
{
  TRACE_STATIC;

  struct addrinfo hints;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = AI_PASSIVE;    // For wildcard IP address
  hints.ai_protocol = 0;          // Any protocol
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;


  struct addrinfo *results;
  int status = getaddrinfo(host.c_str(), port.c_str(), &hints, &results);

  if (status != 0) {
    LOG( Logger::ERR, std::string("Error at network address translation: ").
                                  append(gai_strerror(status)).c_str() ) ;
    return false;
  }

  *servinfo = results;
  return true;
}


void Socket::printHostDetails(struct addrinfo *servinfo)
{
  TRACE_STATIC;

  int counter(0);
  for ( struct addrinfo *it = servinfo; it != 0; it = it->ai_next) {

    counter++;
    void *addr;
    std::string ipver;

    if ( it->ai_family == AF_INET) { // IPv4
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)it->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)it->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop( it->ai_family, addr, ipstr, sizeof ipstr );

    LOG( Logger::DEBUG, std::string(TToStr(counter)).append(". address is ").
                                     append(ipver).append(": ").
                                     append(ipstr).c_str() );
  }
}


bool Socket::convertNameInfo(sockaddr* addr,
                             socklen_t addrLen,
                             std::string &retAddr,
                             std::string &retService)
{
  TRACE_STATIC;

  char hostBuffer[256];
  char serviceBuffer[256];

  int status = getnameinfo( addr, addrLen,
                            hostBuffer, sizeof(hostBuffer),
                            serviceBuffer, sizeof(serviceBuffer),
                            NI_NAMEREQD );

  if ( status != 0 ) {
    LOG( Logger::WARNING, std::string("Could not resolve hostname. ").
                            append(gai_strerror(status)).c_str() );
    return false;
  }

  retAddr.assign(hostBuffer);
  retService.assign(serviceBuffer);
  return true;
}