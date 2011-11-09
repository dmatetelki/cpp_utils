#include "TcpClient.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h> // inet_ntop


TcpClient::TcpClient( const std::string host,
                      const std::string port )
  : m_socket(-1)
  , m_host(host)
  , m_port(port)
  , m_connected(false)
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

  if ( !connectToHost() )
    return false;

  m_connected = true;
  return true;
}


bool TcpClient::disconnect()
{
  TRACE;

  if ( !closeSocket() )
    return false;

  return true;
}


bool TcpClient::send(const std::string msg)
{
  TRACE;

  ssize_t n = write(m_socket, msg.c_str(), msg.length());
  if (n == -1) {
    LOG( Logger::ERR, errnoToString("ERROR writing to socket. ").c_str() );
    return false;
  }

  return true;
}


bool TcpClient::receive(std::string &reply)
{
  TRACE;

  char buffer[256];
  ssize_t n = read(m_socket, buffer, 255);
  if (n == -1) {
    LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
    return false;
  }
  reply = std::string(buffer, n);

  return true;
}


bool TcpClient::openSocket()
{
  TRACE;

  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if ( m_socket == -1 ) {
    LOG( Logger::ERR,  errnoToString("ERROR opening socket. ").c_str() );
    return false;
  }

  return true;
}


bool TcpClient::closeSocket()
{
  TRACE;

  /// @note are the return values of shutdown and close meaningful?
  //   if ( shutdown(m_socket, SHUT_RDWR) == -1 ) {
  //     LOG( Logger::ERR, errnoToString("ERROR shuting down socket. ").c_str() );
  //     return false;
  //   }
  //
  //   if ( close(m_socket) == -1 ) {
  //     LOG( Logger::ERR, errnoToString("ERROR closing socket. ").c_str() );
  //     return false;
  //   }

  shutdown(m_socket, SHUT_RDWR);
  close(m_socket);
  m_socket = -1;

  return true;
}

bool TcpClient::connectToHost()
{
  TRACE;

  struct addrinfo *results(0);
  if ( !getHostInfo(&results) )
    return false;

  printHostDetails(results);

  if ( !connectToFirstAddress(results) )
    return false;

  freeaddrinfo(results);
  return true;
}


bool TcpClient::getHostInfo(struct addrinfo **servinfo)
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
  int status = getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &results);

  if (status != 0) {
    LOG( Logger::ERR, std::string("Error at network address translation: ").
                                  append(gai_strerror(status)).c_str() ) ;
    return false;
  }

  *servinfo = results;
  return true;
}


void TcpClient::printHostDetails(struct addrinfo *servinfo)
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


bool TcpClient::connectToFirstAddress(struct addrinfo *servinfo)
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
