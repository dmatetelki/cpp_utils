#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP


#include "Connection.hpp"
#include "Thread.hpp"
#include "Poll.hpp"


#include <string>
#include <stddef.h> // size_t


class TcpClient
{
private:

  class PollerThread : public Thread
                     , public Poll
  {
  public:

    PollerThread( TcpClient* data );

    void stopPoller();

  protected:

    // overridig poll's behaviour
    virtual void acceptClient();

    // overridig poll's behaviour
    virtual void handleClient( const int );

  private:

    PollerThread(const PollerThread&);
    PollerThread& operator=(const PollerThread&);

    void* run();

    TcpClient   *m_tcpClient;

  };  // class PollerThread


public:

  TcpClient ( const std::string   host,
              const std::string   port,
              Message            *message );

  virtual ~TcpClient();

  bool connect();
  void disconnect();

  bool send( const void* msg, const size_t msgLen );

  bool isPolling() const;

private:

  TcpClient(const TcpClient& );
  TcpClient& operator=(const TcpClient& );


  Connection     m_connection;
  PollerThread   m_watcher;

};

#endif // TCP_CLIENT_HPP
