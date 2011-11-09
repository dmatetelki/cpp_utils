#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


class TcpClient
{

public:

  TcpClient ( const std::string host,
              const std::string port );

  ~TcpClient();

  bool connect();
  bool disconnect();

  bool send(const std::string msg);
  bool receive(std::string &reply);


private:

  bool openSocket();
  bool closeSocket();

  bool connectToHost();
  bool getHostInfo(struct addrinfo **servinfo);
  void printHostDetails(struct addrinfo *servinfo);
  bool connectToFirstAddress(struct addrinfo *servinfo);

  int         m_socket;
  std::string m_host;
  std::string m_port;
  bool        m_connected;

};

#endif // TCP_CLIENT_HPP
