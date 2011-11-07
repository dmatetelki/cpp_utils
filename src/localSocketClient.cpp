#include "localSocketClient.hpp"

#include <string.h>
#include <iostream>

LocalSocketClient::LocalSocketClient(const std::string sockPath)
  : SocketClient( AF_UNIX, SOCK_STREAM )
  , m_sockPath(sockPath)
  , m_remote()
{

}


LocalSocketClient::~LocalSocketClient()
{

}


bool LocalSocketClient::connectToPeer(void)
{
  m_remote.sun_family = AF_UNIX;
  strcpy(m_remote.sun_path, m_sockPath.c_str());
  int len = strlen(m_remote.sun_path) + sizeof(m_remote.sun_family);

  if ( ::connect(m_socketfd, (struct sockaddr *)&m_remote, len) == -1) {
    std::cerr << errorToString("ERROR connecting to peer. ") << std::endl;
    return false;
  }

  m_connected = true;
  return true;
}
