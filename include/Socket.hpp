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
  bool bindToHost(const std::string host,
                  const std::string port );

  static bool convertNameInfo( sockaddr* addr,
                               socklen_t addrLen,
                               std::string &retAddr,
                               std::string &retService);

protected:

  int       m_socket;

  int       m_domain;
  int       m_type;
  int       m_protocol;

  sockaddr  m_addr;
  socklen_t m_addrLen;


private:

  bool connectToFirstAddress(struct addrinfo *servinfo);
  bool bindToFirstAddress(struct addrinfo *servinfo);

  static bool getHostInfo(const std::string host,
                          const std::string port,
                          struct addrinfo **servinfo);
  static void printHostDetails(struct addrinfo *servinfo);

};

#endif // SOCKET_HPP
