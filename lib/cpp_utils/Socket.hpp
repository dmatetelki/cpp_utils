#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>

class Socket
{
public:

  Socket( const int domain,
          const int type,
          const int protocol = 0 );

  Socket( const int socket );

  virtual ~Socket();

  bool createSocket();
  bool closeSocket();

  bool connect(addrinfo *servinfo);
  bool bind(addrinfo *servinfo);
  bool listen( const int maxPendingQueueLen = 64 );
  bool accept( int client_socket );

  bool send( const void *message, const int lenght );
  bool receive ( void* buffer, const int bufferLen, ssize_t *msgLen );

  void getPeerName(std::string &host, std::string &port);
  int getSocket() const;

protected:

  int       m_socket;

  int       m_domain;
  int       m_type;
  int       m_protocol;
};

#endif // SOCKET_HPP
