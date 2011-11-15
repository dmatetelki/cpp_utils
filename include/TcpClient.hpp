#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "TcpConnection.hpp"
#include "Thread.hpp"
#include "Poll.hpp"

#include <string>


class TcpClient
{
private:

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

    TcpClient  &m_tcpClient;
  };


public:

  TcpClient ( const std::string host,
              const std::string port );

  virtual ~TcpClient();

  bool connect();
  void disconnect();

  bool send( const void* message, const int length );

private:

//   virtual void onDisconnect() = 0;

  TcpConnection m_connection;
  WatcherThread m_watcher;

};

#endif // TCP_CLIENT_HPP


