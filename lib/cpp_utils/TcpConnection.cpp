#include "TcpConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "AddrInfo.hpp"


TcpConnection::TcpConnection (  const std::string   host,
                                const std::string   port,
                                Message            *message,
                                const size_t        bufferLength )
  : StreamConnection(host, port)
  , m_socket(AF_INET, SOCK_STREAM) // or AF_INET6 for IPv6
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
  , m_state(CLOSED)
{
  TRACE;
  m_socket.createSocket();
  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


TcpConnection::~TcpConnection()
{
  TRACE;

  if (m_state == OPEN)
    disconnect();

  delete[] m_buffer;
}


Connection* TcpConnection::clone(const int socket)
{
  TcpConnection *tcpConnection = new TcpConnection(socket,
                                                   m_message->clone(),
                                                   m_bufferLength );

  return tcpConnection;
}


bool TcpConnection::connect()
{
  TRACE;

  AddrInfo addrInfo;
  if (!addrInfo.getHostInfo(m_host, m_port))
    return false;

  addrInfo.printHostDetails();

  if (!m_socket.connect(addrInfo[0]))
    return false;

  std::string address, service;
  if ( AddrInfo::convertNameInfo( addrInfo[0], address, service) ) {
    LOG_BEGIN(Logger::INFO)
      LOG_PROP("Host", address)
      LOG_PROP("Port", service)
      LOG_PROP("Socket", m_socket.getSocket())
    LOG_END("Connected to peer.");
  }

  m_state = OPEN;
  return true;
}


bool TcpConnection::bind()
{
  TRACE;

  AddrInfo addrInfo;
  if (!addrInfo.getHostInfo(m_host, m_port))
    return false;

  addrInfo.printHostDetails();

  if (!m_socket.bind(addrInfo[0]))
    return false;

  std::string address, service;
  if ( AddrInfo::convertNameInfo( addrInfo[0], address, service) ) {
    LOG_BEGIN(Logger::INFO)
      LOG_PROP("Host", address)
      LOG_PROP("Port", service)
    LOG_END("Binded to socket.");
  }
  return true;
}


bool TcpConnection::listen( const int maxPendingQueueLen )
{
  TRACE;

  if (m_socket.listen(maxPendingQueueLen)) {
    m_state = OPEN;
    return true;
  }

  return false;
}


bool TcpConnection::accept(int& client_socket)
{
  TRACE;
  return m_socket.accept(client_socket);
}


bool TcpConnection::disconnect()
{
  TRACE;
  m_state = CLOSED;
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
  if (!m_socket.receive(m_buffer, m_bufferLength, &length))
    return false;

  if (length == 0) {
    LOG_BEGIN(Logger::INFO)
      LOG_PROP("Host", m_host)
      LOG_PROP("Port", m_port)
      LOG_PROP("Socket", m_socket.getSocket())
    LOG_END("Connection closed by peer.");
    return false;
  }

  LOG_BEGIN(Logger::INFO)
    LOG_PROP("Host", m_host)
    LOG_PROP("Port", m_port)
    LOG_PROP("Socket", m_socket.getSocket())
    LOG_PROP("Bytes", length)
  LOG_END("Received message from peer.");

  return m_message->buildMessage( (void*)m_buffer, (size_t)length);
}


int TcpConnection::getSocket() const
{
  TRACE;
  return m_socket.getSocket();
}


void TcpConnection::setState(const State state)
{
  TRACE;
  m_state = state;
}


bool TcpConnection::closed() const
{
  TRACE;
  return m_state == CLOSED;
}


Message* TcpConnection::getMessage() const
{
  TRACE;
  return m_message;
}


size_t TcpConnection::getBufferLength() const
{
  TRACE;
  return m_bufferLength;
}


TcpConnection::TcpConnection (  const int      socket,
                                Message       *message,
                                const size_t   bufferLength )
  : StreamConnection("invalid", "invalid")
  , m_socket(socket)
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
  , m_state(OPEN)
{
  TRACE;

  std::string host, port;
  m_socket.getPeerName(host, port);
  setHost(host);
  setPort(port);

  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}
