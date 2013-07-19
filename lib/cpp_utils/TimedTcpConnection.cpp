#include "TimedTcpConnection.hpp"

#include "Logger.hpp"


TimedTcpConnection::TimedTcpConnection(const std::string host,
                                       const std::string port,
                                       Message *message,
                                       const size_t bufferLength,
                                       const unsigned long timeOutSec)
  : StreamConnection(host, port)
  , TimerUser()
  , m_tcpConnection(new TcpConnection(host, port, message, bufferLength))
  , m_timeOutSec(timeOutSec)
{
  TRACE;
}


TimedTcpConnection::~TimedTcpConnection()
{
  TRACE;
  delete m_tcpConnection;
}


void TimedTcpConnection::timerExpired()
{
  TRACE;
  m_tcpConnection->setState(TcpConnection::CLOSED);
}


Connection* TimedTcpConnection::clone(const int socket)
{
  Connection *conn = m_tcpConnection->clone(socket);

  TimedTcpConnection *timedTcpConnection = new TimedTcpConnection(
                                        dynamic_cast<TcpConnection*>(conn),
                                        m_timeOutSec);

  return timedTcpConnection;
}


bool TimedTcpConnection::connect()
{
  TRACE;

  startTimer(m_timeOutSec);
  return m_tcpConnection->connect();
}


bool TimedTcpConnection::bind()
{
  TRACE;
  return m_tcpConnection->bind();
}


bool TimedTcpConnection::listen( const int maxPendingQueueLen )
{
  TRACE;
  return m_tcpConnection->listen(maxPendingQueueLen);
}


bool TimedTcpConnection::accept(int& client_socket)
{
  TRACE;
  return m_tcpConnection->accept(client_socket);
}


bool TimedTcpConnection::disconnect()
{
  TRACE;

  stopTimer();
  return m_tcpConnection->disconnect();
}


bool TimedTcpConnection::send(const void* message, const size_t length)
{
  TRACE;

  startTimer(m_timeOutSec);
  return m_tcpConnection->send(message, length);
}


bool TimedTcpConnection::receive()
{
  TRACE;

  startTimer(m_timeOutSec);
  return m_tcpConnection->receive();
}


int TimedTcpConnection::getSocket() const
{
  TRACE;
  return m_tcpConnection->getSocket();
}


bool TimedTcpConnection::closed() const
{
  TRACE;
  return m_tcpConnection->closed();
}


TimedTcpConnection::TimedTcpConnection(TcpConnection *tcpConnection,
                                       const unsigned long timeOutSec)
  : StreamConnection("invalid", "invalid")
  , TimerUser()
  , m_tcpConnection(tcpConnection)
  , m_timeOutSec(timeOutSec)
{
  TRACE;

  setHost(m_tcpConnection->getHost());
  setPort(m_tcpConnection->getPort());
}
