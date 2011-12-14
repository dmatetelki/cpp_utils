#include "AddrInfo.hpp"

#include "Logger.hpp"

#include <arpa/inet.h> // inet_ntop


AddrInfo::AddrInfo()
  : m_addrInfo(0)
{
  TRACE;
}


AddrInfo::~AddrInfo()
{
  TRACE;

  if (m_addrInfo != 0)
    freeaddrinfo(m_addrInfo);
}


bool AddrInfo::getHostInfo( const std::string host,
                            const std::string port )
{
  TRACE;

  addrinfo hints;

  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket
  hints.ai_flags = AI_PASSIVE;    // For wildcard IP address
  hints.ai_protocol = 0;          // Any protocol
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;


  int status = getaddrinfo(host.c_str(), port.c_str(), &hints, &m_addrInfo);

  if (status != 0) {
    LOG_BEGIN(Logger::ERR)
      LOG_PROP("Error message: ", gai_strerror(status))
    LOG_END("Error at network address translation.");

    return false;
  }

  return true;
}


addrinfo* AddrInfo::operator[](const unsigned int pos)
{
  if (m_addrInfo == 0) {
    LOG(Logger::ERR, "Inner address info structure is null.");
    return 0;
  }

  unsigned int counter(0);
  addrinfo *it(0);
  for ( it = m_addrInfo; it != 0 && counter != pos; it = it->ai_next)
    ++counter;

  if (it == 0) {
    LOG(Logger::ERR, "Position is out of range.");
    return 0;
  }

  return it;
}


void AddrInfo::printHostDetails() const
{
  TRACE;

  int counter(0);
  for ( addrinfo *it = m_addrInfo; it != 0; it = it->ai_next) {

    counter++;
    void *addr;
    std::string ipver;

    if ( it->ai_family == AF_INET) { // IPv4
      sockaddr_in *ipv4 = (sockaddr_in *)it->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    } else { // IPv6
      sockaddr_in6 *ipv6 = (sockaddr_in6 *)it->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    char ipstr[INET6_ADDRSTRLEN];
    inet_ntop( it->ai_family, addr, ipstr, sizeof ipstr );

    LOG_BEGIN(Logger::DEBUG)
      LOG_PROP("Address index", counter)
      LOG_PROP("IP version", ipver)
      LOG_PROP("IP", ipstr)
    LOG_END("Host addresses.");
  }
}


bool AddrInfo::convertNameInfo(const addrinfo *addrInfo,
                               std::string &retAddr,
                               std::string &retService)
{
  TRACE_STATIC;

  char hostBuffer[256];
  char serviceBuffer[256];

  int status = getnameinfo( addrInfo->ai_addr, addrInfo->ai_addrlen,
                            hostBuffer, sizeof(hostBuffer),
                            serviceBuffer, sizeof(serviceBuffer),
                            NI_NAMEREQD );

  if ( status != 0 ) {
    LOG_BEGIN(Logger::WARNING)
      LOG_PROP("Error message: ", gai_strerror(status))
    LOG_END_STATIC("Could not resolve hostname.");

    return false;
  }

  retAddr.assign(hostBuffer);
  retService.assign(serviceBuffer);
  return true;
}
