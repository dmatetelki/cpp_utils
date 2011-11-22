#include "Connection.hpp"

#include "Logger.hpp"
#include "Common.hpp"


Connection::Connection (  const int      socket,
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
}


Connection::Connection (  const std::string   host,
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
}


Connection::~Connection()
{
  TRACE;
  m_socket.closeSocket();
  delete[] m_buffer;
}


Connection* Connection::create(const int socket)
{
  TRACE;
  Connection *conn = new Connection( socket,
                                     m_message->clone(),
                                     m_bufferLength);
  conn->m_message->setConnection(conn);
  return conn;
}


bool Connection::connectToHost()
{
  TRACE;
  return m_socket.connectToHost(m_host, m_port);
}


bool Connection::bindToHost()
{
  TRACE;
  return m_socket.bindToHost(m_host, m_port);
}


bool Connection::listen( const int maxPendingQueueLen)
{
  TRACE;
  return m_socket.listen( maxPendingQueueLen );
}


void Connection::closeConnection()
{
  TRACE;
  m_socket.closeSocket();
}


bool Connection::send( const void* message, const size_t length )
{
  TRACE;
  return m_socket.send( message, length );
}

bool Connection::receive()
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


int Connection::getSocket() const
{
  TRACE;
  return m_socket.getSocket();
}


std::string Connection::getHost() const
{
  TRACE;
  return m_host;
}


std::string Connection::getPort() const
{
  TRACE;
  return m_port;
}
