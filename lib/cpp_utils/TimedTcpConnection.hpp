#ifndef TIMED_TCP_CONNECTION_HPP
#define TIMED_TCP_CONNECTION_HPP


#include "StreamConnection.hpp"
#include "TimerUser.hpp"
#include "TcpConnection.hpp"


/** @brief Inactivity monitored TCP connection.
 *
 * The timer is created at:
 * - ctor, clone
 *
 * The timer is restarted after:
 * - connect, send, receive
 *
 * The timer is destroyed at:
 * - dtor, disconnect
 */

class TimedTcpConnection : public StreamConnection
                         , public TimerUser
{
public:

  TimedTcpConnection(const std::string host,
                     const std::string port,
                     Message *message,
                     const size_t bufferLength = 1024,
                     const unsigned long timeOutSec = 30);

  virtual ~TimedTcpConnection();

  // inherited from TimerUser
  virtual void timerExpired();

  Connection* clone(const int socket);

  /// @todo mention inheritance
  bool connect();
  bool disconnect();

  bool send( const void* message, const size_t length );
  bool receive();

  int getSocket() const;

  bool bind();
  bool listen( const int maxPendingQueueLen = 64 );
  bool accept(int& client_socket);

  bool closed() const;

private:

  TimedTcpConnection(TcpConnection *tcpConnection,
                     const unsigned long timeOutSec = 30);

  TimedTcpConnection(const TimedTcpConnection&);
  TimedTcpConnection& operator=(const TimedTcpConnection&);

  TcpConnection *m_tcpConnection;
  unsigned long m_timeOutSec;

};


#endif // TIMED_TCP_CONNECTION_HPP