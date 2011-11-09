#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>

class Socket
{
public:

  Socket(const int domain,
         const int type,
         const int protocol = 0);
  virtual ~Socket();

  bool openSocket();
  void closeSocket();

  bool connectToHost(const std::string host,
                     const std::string port);
  bool getHostInfo(const std::string host,
                   const std::string port,
                   struct addrinfo **servinfo) const;
  void printHostDetails(struct addrinfo *servinfo) const;
  bool connectToFirstAddress(struct addrinfo *servinfo) const;

protected:

  int m_socket;

  int m_domain;
  int m_type;
  int m_protocol;

};

#endif // SOCKET_HPP
