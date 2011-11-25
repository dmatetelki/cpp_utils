#include "TcpConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"


TcpConnection::TcpConnection (  const int      socket,
                                Message       *message,
                                const size_t   bufferLength )
  : StreamConnection()
  , m_socket(socket)
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
{
  TRACE;

  std::string host, port;
  m_socket.getPeerName(host, port);
  setHost(host);
  setPort(StrToT<int>(port));

  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


TcpConnection::TcpConnection (  const std::string   host,
                                const int           port,
                                Message            *message,
                                const size_t        bufferLength )
  : StreamConnection(host, port)
  , m_socket(AF_INET, SOCK_STREAM) // or AF_INET6 for IPv6
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
{
  TRACE;
  m_socket.createSocket();
  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


TcpConnection::~TcpConnection()
{
  TRACE;
  disconnect();
  delete m_buffer;
}


Connection* TcpConnection::clone(const int socket)
{
  Connection *conn = new TcpConnection(socket,
                                       m_message->clone(),
                                       m_bufferLength );

  return conn;
}


bool TcpConnection::connect()
{
  TRACE;
  return m_socket.connectToHost(m_host, TToStr(m_port));
}


bool TcpConnection::bind()
{
  TRACE;
  return m_socket.bindToHost(m_host, TToStr(m_port));
}


bool TcpConnection::listen( const int maxPendingQueueLen )
{
  TRACE;
  return m_socket.listen( maxPendingQueueLen );
}


bool TcpConnection::disconnect()
{
  TRACE;
  if ( getSocket() == -1 )
    return false;

  return m_socket.closeSocket();
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
                      append(m_host).append(":").append(TToStr(m_port)).c_str() );
    }
    return false;
  }

  LOG ( Logger::DEBUG, std::string("Received: ").
                      append(TToStr(length)).append(" bytes from: ").
                      append(m_host).append(":").append(TToStr(m_port)).c_str() );

  return m_message->buildMessage( (void*)m_buffer, (size_t)length);
}


int TcpConnection::getSocket() const
{
  TRACE;

  return m_socket.getSocket();
}
