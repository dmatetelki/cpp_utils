#include "TcpConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <sys/types.h>
#include <sys/socket.h>


TcpConnection::TcpConnection ( const int socket,
                               const int bufferLenght,
                               MessageBuilder* builder
                             )
  : m_socket(socket)
  , m_host()
  , m_port()
  , m_status(CLOSED)
  , m_builder(builder)
  , m_buffer(0)
  , m_bufferLength(bufferLenght)

{
  TRACE;

  m_buffer = new unsigned char[m_bufferLength];
  m_socket.getPeerName(m_host, m_port);
}


TcpConnection::TcpConnection ( const std::string host,
                               const std::string port,
                               const int bufferLenght,
                               MessageBuilder* builder )
  : m_socket(AF_INET, SOCK_STREAM)
  , m_host(host)
  , m_port(port)
  , m_status(CLOSED)
  , m_builder(builder)
  , m_buffer(0)
  , m_bufferLength(bufferLenght)
{
  TRACE;

  m_buffer = new unsigned char[m_bufferLength];
  m_socket.createSocket();
}


TcpConnection::~TcpConnection()
{
  TRACE;

  delete[] m_buffer;
  m_socket.closeSocket();
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


void TcpConnection::closeConnection()
{
  TRACE;

  m_socket.closeSocket();
}


bool TcpConnection::sendMessage( const void* message, const int length )
{
  TRACE;

  return m_socket.send( message, length );
}


int& TcpConnection::getSocket() const
{
  TRACE;

  return m_socket.getSocket();
}


bool TcpConnection::readFromSocket()
{
  TRACE;

  int len = recv(m_socket, m_buffer, m_bufferLength, 0);

  if (len == -1) {
    LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
    return false;
  }

  if (len == 0) {
    LOG( Logger::DEBUG, "Connection closed by peer." );
    return false;
  }

  if ( !m_builder ) {
    onMessageReady(m_buffer, len);
    return true;
  }

  return m_builder->buildMessage(m_buffer, len);
}
