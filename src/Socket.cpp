#include "Socket.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntop

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


Socket::~Socket()
{
  TRACE;
}


bool Socket::openSocket()
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


bool Socket::connectToHost(const std::string host,
                           const std::string port )
{
  TRACE;

  struct addrinfo *results(0);
  if ( !getHostInfo(host, port, &results) )
    return false;

  printHostDetails(results);

  if ( !connectToFirstAddress(results) )
    return false;

  freeaddrinfo(results);
  return true;
}


bool Socket::getHostInfo( const std::string host,
                          const std::string port,
                          struct addrinfo **servinfo) const
{
  TRACE;

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


void Socket::printHostDetails(struct addrinfo *servinfo) const
{
  TRACE;

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


bool Socket::connectToFirstAddress(struct addrinfo *servinfo) const
{
  TRACE;

  for ( struct addrinfo *it = servinfo; it != 0; it = it->ai_next)
    if (::connect(m_socket, it->ai_addr, it->ai_addrlen) != -1) {
      char hostBuffer[256];
      char serviceBuffer[256];
      int status = getnameinfo( it->ai_addr, it->ai_addrlen,
                                hostBuffer, sizeof(hostBuffer),
                                serviceBuffer, sizeof(serviceBuffer),
                                NI_NAMEREQD );

      if ( status != 0 ) {
        LOG( Logger::WARNING, std::string("Could not resolve hostname. ").
                                append(gai_strerror(status)).c_str() );
      } else {
        LOG( Logger::DEBUG, std::string("Connected to ").
                              append(hostBuffer).append(":").
                              append(serviceBuffer).c_str() );
      }
      return true;
    }

  LOG( Logger::ERR, std::string("Could not connect to host,"
                                " connection refused.").c_str() );
  return false;
}