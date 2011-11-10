#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "Socket.hpp"

#include <string>
#include <poll.h>

class TcpServer : public Socket
{
public:

  TcpServer ( const std::string host,
              const std::string port,
              const int maxClients = 5 );

  virtual ~TcpServer();

  bool start();
  void stop();

  virtual void msgArrived(const int clientSocket,
                          const std::string msg) = 0;

private:

  TcpServer(const TcpServer&);
  TcpServer& operator=(const TcpServer&);

  bool receive(const int clientSocket);

  void addFd( int fd, short events );
  void removeFd( int fd );

  std::string  m_host;
  std::string  m_port;
  nfds_t       m_maxclients;
  bool         m_running;
  pollfd      *m_fds;
  nfds_t       m_num_of_fds;
  sockaddr     m_addr;
  socklen_t    m_addrLen;
};

#endif // TCP_SERVER_HPP
