#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Socket.hpp"
#include "Thread.hpp"

#include <string>


class TcpClient : public Socket
{

public:

  TcpClient ( const std::string host,
              const std::string port );

  virtual ~TcpClient();

  bool connect();
  void disconnect();

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

  std::string   m_host;
  std::string   m_port;
  bool          m_connected;
  WatcherThread m_watcher;

};

#endif // TCP_CLIENT_HPP
