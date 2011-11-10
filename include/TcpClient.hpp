#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Socket.hpp"
#include "Thread.hpp"
#include "Poll.hpp"

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
                      , public Poll
  {
  public:
    WatcherThread( TcpClient &data );

    // overringind Poll's accept behaviour
    void acceptClient();
    void handleClient( const int fd );
    bool receive( const int fd );

  private:
    void* run();

    TcpClient &m_tcpClient;
  };

  std::string   m_host;
  std::string   m_port;
  WatcherThread m_watcher;

};

#endif // TCP_CLIENT_HPP
