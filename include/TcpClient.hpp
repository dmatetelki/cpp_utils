#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Thread.hpp"

#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


class TcpClient
{

public:

  TcpClient ( const std::string host,
              const std::string port );

  virtual ~TcpClient();

  bool connect();
  bool disconnect();

  bool send(const std::string msg);


private:

  virtual void msgArrived(const std::string) = 0;
  virtual void onDisconnect() = 0;

  class WatcherThread : public Thread
  {
  public:
    WatcherThread( TcpClient &data );

  private:
    void* run();
    bool receive();

    TcpClient &m_tcpClient;
  };

  bool openSocket();
  bool closeSocket();

  bool connectToHost();
  bool getHostInfo(struct addrinfo **servinfo) const;
  void printHostDetails(struct addrinfo *servinfo) const;
  bool connectToFirstAddress(struct addrinfo *servinfo) const;

  int           m_socket;
  std::string   m_host;
  std::string   m_port;
  bool          m_connected;
  WatcherThread m_watcher;

};

#endif // TCP_CLIENT_HPP
