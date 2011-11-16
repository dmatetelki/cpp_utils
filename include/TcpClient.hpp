#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Logger.hpp"

#include "Connection.hpp"
#include "Thread.hpp"
#include "Poll.hpp"


#include <string>
#include <stddef.h> // size_t


template <typename T>
class TcpClient
{
private:

  template <typename U>
  class PollerThread : public Thread
                     , public Poll<U>
  {
  public:
    PollerThread( TcpClient<U> &data )
      : Poll<U>( &(data.m_connection) )
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

  /// @todo this is unclear and nasty hack
  virtual void acceptClient( const int socket )
  {
    TRACE;

    LOG( Logger::DEBUG, std::string("own socket: ").
          append( TToStr(m_tcpClient.m_connection.getSocket())).
          append( " param socket: ").
          append( TToStr( socket) ).c_str() );

    m_tcpClient.m_connection.receive();
    stopPolling();
  }

  /// @todo this is unclear and nasty hack
  virtual void handleClient( const int socket )
  {
    TRACE;
    LOG( Logger::DEBUG, "Server closed the connection." );
    stopPolling();
  }

  private:

    void* run()
    {
      TRACE;
      startPolling();
      return 0;
    }

    TcpClient<U> &m_tcpClient;

  };  // class PollerThread


public:

  TcpClient ( const std::string   host,
              const std::string   port,
              void               *msgParam = 0 )
    : m_connection (host, port, msgParam)
    , m_watcher(*this)
  {
    TRACE;
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

private:

  TcpClient(const TcpClient& );
  TcpClient& operator=(const TcpClient& );

  Connection<T>& getConnection()
  {
    TRACE;
    return m_connection;
  }

  Connection<T>    m_connection;
  PollerThread<T>  m_watcher;

};

#endif // TCP_CLIENT_HPP


