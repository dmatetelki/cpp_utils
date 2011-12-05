#include "TcpConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "AddrInfo.hpp"


TcpConnection::TcpConnection (  const int      socket,
                                Message       *message,
                                const size_t   bufferLength )
  : StreamConnection("invalid", "invalid")
  , m_socket(socket)
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
{
  TRACE;

  std::string host, port;
  m_socket.getPeerName(host, port);
  setHost(host);
  setPort(port);

  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


TcpConnection::TcpConnection (  const std::string   host,
                                const std::string   port,
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

  AddrInfo addrInfo;
  if (!addrInfo.getHostInfo(m_host, m_port))
    return false;

  addrInfo.printHostDetails();

  if (!m_socket.connect(addrInfo[0]))
    return false;

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

  return true;
}


bool TcpConnection::listen( const int maxPendingQueueLen )
{
  TRACE;
  return m_socket.listen( maxPendingQueueLen );
}


int TcpConnection::accept()
{
  TRACE;
  sockaddr clientAddr;
  socklen_t clientAddrLen;

  /// @todo move accept to Socket
  int client_socket = ::accept( getSocket(), &clientAddr, &clientAddrLen ) ;

  if ( client_socket == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR accepting. ").c_str() );
    return -1;
  }

  return client_socket;
}


bool TcpConnection::disconnect()
{
  TRACE;
//   if ( getSocket() == -1 )
//     return false;

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
    if (length == 0) {
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
