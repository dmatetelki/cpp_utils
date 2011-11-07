#include "socketClient.hpp"

#include <errno.h> // errno
#include <string.h> // strerror

#include <iostream>


SocketClient::SocketClient( const int addrDomain,
                            const int socketType )
  : m_connected(false)
  , m_addrDomain(addrDomain)
  , m_socketType(socketType)
  , m_socketfd(0)
{
}


SocketClient::~SocketClient()
{
  if ( m_connected && close(m_socketfd) == -1 )
    std::cerr << errorToString("ERROR closing socket. ") << std::endl;
}


bool SocketClient::send(const std::string msg)
{
  if ( !m_connected && !this->connect() )
    return false;

  ssize_t n = write(m_socketfd, msg.c_str(), msg.length());
  if (n == -1) {
    std::cerr << errorToString("ERROR writing to socket. ") << std::endl;
    return false;
  } else if ( n < (ssize_t)msg.length() ) {
    std::cerr << "Only a part of msg has been written to socket. " << std::endl;
    return false;
  }

  return true;
}


bool SocketClient::receive(std::string &reply)
{
  if ( !m_connected && !this->connect() )
    return false;

  char buffer[256];
  ssize_t n = read(m_socketfd, buffer, 255);
  if (n == -1) {
    std::cerr << errorToString("ERROR reading from socket. ") << std::endl;
    return false;
  }
  reply = std::string(buffer, n);

  return true;
}


bool SocketClient::connect(void)
{
  m_socketfd = socket(m_addrDomain, m_socketType, 0);
  if ( m_socketfd == -1 ) {
    std::cerr << errorToString("ERROR opening socket. ") << std::endl;
    return false;
  }

  return connectToPeer();
}

std::string SocketClient::errorToString(const char *s) const
{
  return std::string(s).append(strerror(errno));
}