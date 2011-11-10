#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "Socket.hpp"
#include "Poll.hpp"

#include <string>

class TcpServer : public Socket
                , public Poll
{
public:

  TcpServer ( const std::string host,
              const std::string port,
              const int maxClients = 5 );

  virtual ~TcpServer();

  bool start();
  void stop();

  // implements Poll::receive
  bool receive( const int fd );

  virtual void msgArrived(const int clientSocket,
                          const std::string msg) = 0;

private:

  TcpServer(const TcpServer&);
  TcpServer& operator=(const TcpServer&);

  std::string  m_host;
  std::string  m_port;
};

#endif // TCP_SERVER_HPP
