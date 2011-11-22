#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Logger.hpp"

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

    PollerThread( TcpClient* data )
      : Poll( &(data->m_connection) )
      , m_tcpClient(data)
    {
      TRACE;
    }

    void stopPoller()
    {
      TRACE;
      stopPolling();
      stop();
    }

  protected:

    // overridig poll's behaviour
    virtual void acceptClient()
    {
      TRACE;

      m_tcpClient->m_connection.receive();
      stopPolling();
    }

    // overridig poll's behaviour
    virtual void handleClient( const int )
    {
      TRACE;
      LOG( Logger::DEBUG, "Server closed the connection." );
      stopPolling();
    }

  private:

    PollerThread(const PollerThread&);
    PollerThread& operator=(const PollerThread&);

    void* run()
    {
      TRACE;
      startPolling();
      return 0;
    }

    TcpClient   *m_tcpClient;

  };  // class PollerThread


public:

  TcpClient ( const std::string   host,
              const std::string   port,
              Message            *message )
    : m_connection (host, port, message)
    , m_watcher(this)
  {
    TRACE;

    message->setConnection(&m_connection);
  }

  virtual ~TcpClient()
  {
    TRACE;
    disconnect();
  }

  bool connect()
  {
    TRACE;

    if ( !m_connection.connectToHost() )
      return false;

    m_watcher.start();
    return true;
  }

  void disconnect()
  {
    TRACE;

    if ( m_watcher.isRunning() ) {
      m_watcher.stopPoller();
      m_watcher.join();
    }

    m_connection.closeConnection();
  }

  bool send( const void* msg, const size_t msgLen )
  {
    TRACE;
    return m_connection.send(msg, msgLen);
  }

  bool isPolling() const
  {
    TRACE;
    return m_watcher.isPolling();
  }

private:

  TcpClient(const TcpClient& );
  TcpClient& operator=(const TcpClient& );


  Connection     m_connection;
  PollerThread   m_watcher;

};

#endif // TCP_CLIENT_HPP
