#include "inetSocketClient.hpp"

#include <iostream>

#include <string.h> // memset, memcpy


InetSocketClient::InetSocketClient(const std::string host,
                                   const int port)
  : SocketClient( AF_INET, SOCK_STREAM )
  , m_host(host)
  , m_port(port)
  , m_serverAddr()
{

}


InetSocketClient::~InetSocketClient()
{

}


bool InetSocketClient::connectToPeer()
{
  struct hostent *server = gethostbyname(m_host.c_str());
  if (server == 0) {
    std::cerr << errorToString("ERROR, no such host. ") << std::endl;
    return false;
  }

  m_serverAddr.sin_family = AF_INET;
  m_serverAddr.sin_port = htons(m_port);
  memcpy(&m_serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);

  if ( ::connect( m_socketfd,
                  (struct sockaddr *) &m_serverAddr,
                  sizeof(m_serverAddr)) == -1) {
    std::cerr << errorToString("ERROR connecting to peer. ") << std::endl;
    return false;
  }

  m_connected = true;
  return true;
}
