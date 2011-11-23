#include "SocketConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"


SocketConnection::SocketConnection (  const int      socket,
                                      Message       *message,
                                      const size_t   bufferLength )
  : m_socket(socket)
  , m_host()
  , m_port()
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
{
  TRACE;

  m_socket.getPeerName(m_host, m_port);
  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


SocketConnection::SocketConnection (  const std::string   host,
                                      const std::string   port,
                                      Message            *message,
                                      const size_t        bufferLength )
  : m_socket(AF_INET, SOCK_STREAM)
  , m_host(host)
  , m_port(port)
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
{
  TRACE;
  m_socket.createSocket();
  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


SocketConnection::~SocketConnection()
{
  TRACE;
  m_socket.closeSocket();
  delete[] m_buffer;
}


int SocketConnection::getSocket() const
{
  TRACE;
  return m_socket.getSocket();
}


std::string SocketConnection::getHost() const
{
  TRACE;
  return m_host;
}


std::string SocketConnection::getPort() const
{
  TRACE;
  return m_port;
}
