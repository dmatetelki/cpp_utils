#ifndef INET_SOCKET_CLIENT_HPP
#define INET_SOCKET_CLIENT_HPP

#include "socketClient.hpp"

class InetSocketClient : public SocketClient
{
public:

  InetSocketClient( const std::string host,
                    const int port );

  ~InetSocketClient();


private:

  bool connectToPeer(void);

  std::string m_host;
  int m_port;
  struct sockaddr_in m_serverAddr;

};


#endif // INET_SOCKET_CLIENT_HPP
