#include "SslConnection.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>



void SslConnection::init()
{
  TRACE_STATIC;

  SSL_load_error_strings();
  SSL_library_init();
}

void SslConnection::destroy()
{
  TRACE_STATIC;

  ERR_free_strings();
}


SslConnection::SslConnection (  const int      socket,
                                Message       *message,
                                const size_t   bufferLength )
  : SocketConnection(socket, message, bufferLength)
  , m_tcpConnection(socket, 0, 0)
  , m_sslHandle(0)
  , m_sslContext(0)
{
  TRACE;
}


SslConnection::SslConnection (  const std::string   host,
                                const std::string   port,
                                Message            *message,
                                const size_t        bufferLength )
  : SocketConnection(host, port, message, bufferLength)
  , m_tcpConnection(host, port, 0, 0)
  , m_sslHandle(0)
  , m_sslContext(0)
{
  TRACE;
}


SslConnection::~SslConnection()
{
  TRACE;
  closeConnection();
}


SocketConnection* SslConnection::clone(const int socket)
{
  SocketConnection *conn = new SslConnection(socket,
                                             m_message->clone(),
                                             m_bufferLength );

  return conn;
}


bool SslConnection::connectToHost()
{
  TRACE;

  if ( !m_tcpConnection.connectToHost() )
    return false;

  return connect();
}


bool SslConnection::bindToHost()
{
  TRACE;

  if ( !m_tcpConnection.bindToHost() )
    return false;

  return connect();
}


bool SslConnection::listen( const int maxPendingQueueLen )
{
  TRACE;
  return m_tcpConnection.listen(maxPendingQueueLen);
}


void SslConnection::closeConnection()
{
  TRACE;

  /// @note do I have to call this?
  m_tcpConnection.closeConnection();

  int ret = SSL_shutdown(m_sslHandle);

  if ( ret == 0 ) {
    LOG( Logger::INFO, "\"close notify\" alert was sent and the peer's "
                       "\"close notify\" alert was received.");
  }
  else if (ret == 1 ) {
    LOG( Logger::WARNING, "\"The shutdown is not yet finished. "
                          "Calling SSL_shutdown() for a second time...");
    SSL_shutdown(m_sslHandle);
  }
  else if ( ret == 2 ) {
    LOG (Logger::ERR, getSslError("The shutdown was not successful. ").c_str() );
  }

  SSL_free(m_sslHandle);
  SSL_CTX_free(m_sslContext);
}


bool SslConnection::send( const void* message, const size_t length )
{
  TRACE;

  int ret = SSL_write(m_sslHandle, message, length);

  if ( ret > 0 )
    return true;

  unsigned long sslErrNo = ERR_peek_error();
  if ( ret == 0 && sslErrNo == SSL_ERROR_ZERO_RETURN ) {
    LOG( Logger::INFO, "Underlying connection has been closed.");
    return true;
  }

  LOG (Logger::ERR, getSslError("SSL write failed. ").c_str() );
  return false;
}


bool SslConnection::receive()
{
  TRACE;

  int length = SSL_read(m_sslHandle, m_buffer, m_bufferLength);

  if ( length > 0 )
    return m_message->buildMessage( (void*)m_buffer, (size_t)length);

  unsigned long sslErrNo = ERR_peek_error();
  if ( length == 0 && sslErrNo == SSL_ERROR_ZERO_RETURN ) {
    LOG( Logger::INFO, "Underlying connection has been closed.");
    return true;
  }

  LOG (Logger::ERR, getSslError("SSL read failed. ").c_str() );
  return false;
}


bool SslConnection::connect()
{
  TRACE;

  m_sslContext = SSL_CTX_new (SSLv23_client_method ());
  if ( m_sslContext == NULL ) {
    LOG (Logger::ERR, getSslError("Creating SSL context failed. ").c_str() );
    return false;
  }

  m_sslHandle = SSL_new (m_sslContext);
  if ( m_sslHandle == NULL ) {
    LOG (Logger::ERR, getSslError("Creating SSL structure for connection failed. ").c_str() );
    return false;
  }


  if ( !SSL_set_fd (m_sslHandle, m_tcpConnection.getSocket()) ) {
    LOG (Logger::ERR, getSslError("Connect the SSL object with a file descriptor failed. ").c_str() );
    return false;
  }


  if ( SSL_connect (m_sslHandle) != 1 ) {
    LOG (Logger::ERR, getSslError("Handshake with SSL server failed. ").c_str() );
    return false;
  }

  return true;
}


std::string SslConnection::getSslError(const std::string &msg)
{
  char buffer[130];
  unsigned long sslErrNo = ERR_get_error();

  ERR_error_string(sslErrNo, buffer);

  return std::string(msg).append(buffer);
}
