#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP


#include "SocketConnection.hpp"
#include "Thread.hpp"
#include "Poll.hpp"


#include <string>
#include <stddef.h> // size_t


class SocketClient
{
private:

  class PollerThread : public Thread
                     , public Poll
  {
  public:

    PollerThread( SocketClient* data );

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

    SocketClient   *m_tcpClient;

  };  // class PollerThread


public:

  SocketClient (SocketConnection *connection );

  virtual ~SocketClient();

  bool connect();
  void disconnect();

  bool send( const void* msg, const size_t msgLen );

  bool isPolling() const;

private:

  SocketClient(const SocketClient& );
  SocketClient& operator=(const SocketClient& );


  SocketConnection  *m_connection;
  PollerThread       m_watcher;

};

#endif // SOCKET_CLIENT_HPP
