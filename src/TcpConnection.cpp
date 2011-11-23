#include "TcpConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"


TcpConnection::TcpConnection (  const int      socket,
                                Message       *message,
                                const size_t   bufferLength )
  : SocketConnection(socket, message, bufferLength)
{
  TRACE;
}


TcpConnection::TcpConnection (  const std::string   host,
                                const std::string   port,
                                Message            *message,
                                const size_t        bufferLength )
  : SocketConnection(host, port, message, bufferLength)
{
  TRACE;
}


TcpConnection::~TcpConnection()
{
  TRACE;
}


SocketConnection* TcpConnection::clone(const int socket)
{
  SocketConnection *conn = new TcpConnection(socket,
                                             m_message->clone(),
                                             m_bufferLength );

  return conn;
}


bool TcpConnection::connectToHost()
{
  TRACE;
  return m_socket.connectToHost(m_host, m_port);
}


bool TcpConnection::bindToHost()
{
  TRACE;
  return m_socket.bindToHost(m_host, m_port);
}


bool TcpConnection::listen( const int maxPendingQueueLen )
{
  TRACE;
  return m_socket.listen( maxPendingQueueLen );
}


void TcpConnection::closeConnection()
{
  TRACE;
  m_socket.closeSocket();
}


bool TcpConnection::send( const void* message, const size_t length )
{
  TRACE;
  return m_socket.send( message, length );
}


bool TcpConnection::receive()
{
  TRACE;

  ssize_t length;
  if ( !m_socket.receive(m_buffer, m_bufferLength, &length) ) {
    if (length == -1) {
      LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
    }
    else if (length == 0) {
      LOG( Logger::INFO, std::string("Connection closed by ").
                      append(m_host).append(":").append(m_port).c_str() );
    }
  return false;
  }

  LOG ( Logger::DEBUG, std::string("Received: ").
                      append(TToStr(length)).append(" bytes from: ").
                      append(m_host).append(":").append(m_port).c_str() );

  return m_message->buildMessage( (void*)m_buffer, (size_t)length);
}
