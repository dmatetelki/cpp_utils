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
  OpenSSL_add_all_algorithms();
}

void SslConnection::destroy()
{
  TRACE_STATIC;

  ERR_free_strings();
}


SslConnection::SslConnection (  const std::string   host,
                                const std::string   port,
                                Message            *message,
                                const size_t        bufferLength,
                                bool                bidirectional_shutdown
                             )
  : StreamConnection(host, port)
  , m_timedTcpConnection(new TimedTcpConnection(host, port, message, 0))
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
  , m_sslHandle(0)
  , m_sslContext(0)
  , m_bidirectional_shutdown(bidirectional_shutdown)
{
  TRACE;
  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


SslConnection::~SslConnection()
{
  TRACE;
  disconnect();
  delete m_buffer;
  delete m_timedTcpConnection;
}


Connection* SslConnection::clone(const int socket)
{
  TRACE;

  Connection* conn = m_timedTcpConnection->clone(socket);

  SslConnection *sslConn = new SslConnection(
                                        dynamic_cast<TimedTcpConnection*>(conn),
                                        m_message->clone(),
                                        m_bufferLength);
  sslConn->setHandle(m_sslHandle);
  return sslConn;
}


bool SslConnection::connect()
{
  TRACE;

  if ( !m_timedTcpConnection->connect() )
    return false;

  if ( SSL_set_fd(m_sslHandle, m_timedTcpConnection->getSocket() ) == 0 ) {
    LOG( Logger::ERR, getSslError("SSL set connection socket failed. ").c_str() );
    return -1;
  }

  if ( SSL_connect (m_sslHandle) != 1 ) {
    LOG (Logger::ERR, getSslError("SSL handshake failed. ").c_str() );
    return false;
  }

  showCertificates();

  return true;
}


bool SslConnection::bind()
{
  TRACE;

  return m_timedTcpConnection->bind();
}


bool SslConnection::listen( const int maxPendingQueueLen )
{
  TRACE;
  return m_timedTcpConnection->listen(maxPendingQueueLen);
}


bool SslConnection::accept( int& client_socket )
{
  TRACE;

  if(!m_timedTcpConnection->accept(client_socket))
    return false;

  if ( SSL_set_fd(m_sslHandle, client_socket) == 0 ) {
    LOG( Logger::ERR, getSslError("SSL set connection socket failed. ").c_str() );
    return false;
  }

  if ( SSL_accept(m_sslHandle) == -1 ) {
    LOG( Logger::ERR, getSslError("SSL accept failed. ").c_str() );
    return false;
  }

  return true;
}

/// @todo this function shall be refactored
bool SslConnection::disconnect()
{
  TRACE;

  if ( m_sslHandle == 0 || m_sslContext == 0 )
    return false;

  int ret = SSL_shutdown(m_sslHandle);
  if ( ret == 0 ) {
    LOG( Logger::INFO, "\"close notify\" alert was sent, "
                       "shutdown is not yet finished");
    if (m_bidirectional_shutdown) {
      int ret2 = SSL_shutdown(m_sslHandle);
      if ( ret2 == 0 ) {
        LOG( Logger::ERR, "Shutdown is not finished, giving up.");
      } else if ( ret2 == 1 ) {
        LOG( Logger::INFO, "Peer's \"close notify\" alert reply was received.");
      } else if ( ret2 < 0 ) {
        LOG (Logger::INFO, "No reply, the peer probably closed the underlying connection." );
      }
    }
  }
  else if (ret == 1 ) {
    LOG( Logger::INFO, "\"close notify\" alert was sent"
                       " and the peer's \"close notify\" alert was received.");
  }
  else if ( ret < 0 ) {
    LOG (Logger::ERR, (getSslError("The shutdown was not successful. ")
      + "system error: " + std::string(strerror(errno))).c_str()

    );
  }

  /// @note I have to check the ref count?! This stinks
  if (m_sslHandle && m_sslHandle->references > 0)
    SSL_free(m_sslHandle);

  if (m_sslHandle && m_sslContext->references > 0)
    SSL_CTX_free(m_sslContext);

  m_sslHandle = 0;
  m_sslContext = 0;

  /// @note do I have to call this?
  if ( m_timedTcpConnection->getSocket() != -1 )
    m_timedTcpConnection->disconnect();

  return true;
}


bool SslConnection::initServerContext( const std::string certificateFile,
                                       const std::string privateKeyFile )
{
  TRACE;

  m_sslContext = SSL_CTX_new (TLSv1_2_server_method ());
  if ( m_sslContext == NULL ) {
    LOG (Logger::ERR, getSslError("Creating SSL context failed. ").c_str() );
    return false;
  }

  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_SSLv2);
  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_SSLv3);
  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_TLSv1);
  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_TLSv1_1);

  if ( !loadCertificates(certificateFile, privateKeyFile) )
    return false;

  return initHandle();
}


bool SslConnection::initClientContext()
{
  TRACE;

  m_sslContext = SSL_CTX_new (TLSv1_2_client_method ());
  if ( m_sslContext == NULL ) {
    LOG (Logger::ERR, getSslError("Creating SSL context failed. ").c_str() );
    return false;
  }

  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_SSLv2);
  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_SSLv3);
  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_TLSv1);
  SSL_CTX_set_options(m_sslContext, SSL_OP_NO_TLSv1_1);

  return initHandle();
}


bool SslConnection::send( const void* message, const size_t length )
{
  TRACE;

  int ret = SSL_write(m_sslHandle, message, length);

  if ( ret > 0 )
    return true;

  unsigned long sslErrNo = ERR_get_error();
  if ( ret == 0 && sslErrNo == SSL_ERROR_ZERO_RETURN ) {
    LOG( Logger::INFO, "SSL connection has been closed, cannot write.");
    return false;
  }

  LOG (Logger::ERR, getSslError("SSL write failed. ").c_str() );
  return false;
}


bool SslConnection::receive()
{
  TRACE;

  int ret = SSL_read(m_sslHandle, m_buffer, m_bufferLength);

  if ( ret > 0 )
    return m_message->buildMessage( (void*)m_buffer, (size_t)ret);

  unsigned long sslErrNo = ERR_get_error();
  if ( ret == 0  && (sslErrNo == SSL_ERROR_ZERO_RETURN ||
                     sslErrNo == SSL_ERROR_NONE) ) {
    LOG( Logger::INFO, "SSL connection has been closed, cannot read.");
    return false;
  }

  LOG (Logger::ERR, (getSslError("The shutdown was not successful. ")
      + "system error: " + std::string(strerror(errno))).c_str() );

  return false;
}


bool SslConnection::closed() const
{
  TRACE;
  return m_timedTcpConnection->closed();
}


int SslConnection::getSocket() const
{
  TRACE;
  return m_timedTcpConnection->getSocket();
}


SslConnection::SslConnection(TimedTcpConnection* timedTcpConnection,
                             Message*            message,
                             const size_t        bufferLength,
                             bool                bidirectional_shutdown
                            )
  : StreamConnection("invalid", "invalid")
  , m_timedTcpConnection(timedTcpConnection)
  , m_message(message)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
  , m_sslHandle(0)
  , m_sslContext(0)
  , m_bidirectional_shutdown(bidirectional_shutdown)
{
  TRACE;

  setHost(m_timedTcpConnection->getHost());
  setPort(m_timedTcpConnection->getPort());

  m_buffer = new unsigned char[m_bufferLength];
  m_message->setConnection(this);
}


bool SslConnection::initHandle()
{
  TRACE;

  m_sslHandle = SSL_new (m_sslContext);
  if ( m_sslHandle == NULL ) {
    LOG (Logger::ERR, getSslError("Creating SSL structure for connection failed. ").c_str() );
    return false;
  }

  return true;
}


void SslConnection::setHandle(SSL *handle)
{
  TRACE;

  m_sslHandle = handle;
}


std::string SslConnection::getSslError(const std::string &msg)
{
  TRACE;

  char buffer[130];
  unsigned long sslErrNo = ERR_get_error();

  ERR_error_string(sslErrNo, buffer);

  return std::string(msg).append(buffer);
}


bool SslConnection::loadCertificates( const std::string certificateFile,
                                      const std::string privateKeyFile )
{
  TRACE;

  if ( SSL_CTX_use_certificate_file(m_sslContext,
                                    certificateFile.c_str(),
                                    SSL_FILETYPE_PEM) != 1 ) {
    LOG (Logger::ERR, getSslError("SSL certificate file loading failed. ").c_str() );
    return false;
  }

  if ( SSL_CTX_use_PrivateKey_file(m_sslContext,
                                   privateKeyFile.c_str(),
                                   SSL_FILETYPE_PEM) != 1 ) {
    LOG (Logger::ERR, getSslError("SSL private Key file loading failed. ").c_str() );
    return false;
  }

  if ( SSL_CTX_check_private_key(m_sslContext) != 1 ) {
    LOG( Logger::ERR, getSslError("Private key does not match the public certificate.\n").c_str() );
    return false;
  }

  return true;
}


void SslConnection::showCertificates()
{
  TRACE;

  X509 *cert = SSL_get_peer_certificate(m_sslHandle);
  if (cert == NULL) {
    LOG( Logger::ERR, "SSL get peer certificate failed. " );
    return;
  }

  char *line;
  line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
  LOG( Logger::DEBUG, std::string("Server certificate, subject: \"").append(line).append("\"").c_str() );
  free(line);

  line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
  LOG( Logger::DEBUG, std::string("Server certificate, issuer: \"").append(line).append("\"").c_str() );
  free(line);

  X509_free(cert);
  return;
}
