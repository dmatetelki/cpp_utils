#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class SocketClient
{
public:

  SocketClient( const int addrDomain,
                const int socketType);

  virtual ~SocketClient();

  bool send(const std::string msg);
  bool receive(std::string &reply);

protected:

  bool connect(void);
  virtual bool connectToPeer(void) = 0;
  std::string errorToString(const char *s) const;

  bool m_connected;
  int m_addrDomain;
  int m_socketType;
  int m_socketfd;
};


#endif // SOCKET_CLIENT_HPP
