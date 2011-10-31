#ifndef LOCAL_SOCKET_CLIENT_HPP
#define LOCAL_SOCKET_CLIENT_HPP

#include "socketClient.hpp"

#include <sys/un.h>

class LocalSocketClient : public SocketClient
{
public:

  LocalSocketClient( const std::string sockPath);

  ~LocalSocketClient();


private:

  bool connectToPeer(void);

  std::string m_sockPath;
  struct sockaddr_un m_remote;

};


#endif // LOCAL_SOCKET_CLIENT_HPP
