#ifndef SSL_CONNECTION_HPP
#define SSL_CONNECTION_HPP


#include "StreamConnection.hpp"
#include "TimedTcpConnection.hpp"

#include <string>
#include <openssl/ssl.h>



/// @note Call init/destroy before/after usage
class SslConnection : public StreamConnection
{
public:

  static void init();
  static void destroy();

  SslConnection ( const std::string   host,
                  const std::string   port,
                  Message            *message,
                  const size_t        bufferLength = 1024 );

  virtual ~SslConnection();

  Connection* clone(const int socket);

  bool connect();
  bool disconnect();

  bool initServerContext( const std::string certificateFile,
                          const std::string privateKeyFile );
  bool initClientContext();

  bool send( const void* message, const size_t length );
  bool receive();

  bool bind();
  bool listen( const int maxPendingQueueLen = 64 );
  bool accept( int& client_socket );

  bool closed() const;
  int getSocket() const;

private:

  SslConnection ( TimedTcpConnection *timedTcpConnection,
                  Message       *message,
                  const size_t   bufferLength = 1024 );

  SslConnection(const SslConnection&);
  SslConnection& operator=(const SslConnection&);

  bool initHandle();
  void setHandle(SSL *handle);
  std::string getSslError(const std::string &msg);
  bool loadCertificates( const std::string certificateFile,
                         const std::string keyFile );
  void showCertificates();


  TimedTcpConnection *m_timedTcpConnection;
  Message *m_message;
  unsigned char *m_buffer;
  size_t m_bufferLength;
  SSL *m_sslHandle;
  SSL_CTX *m_sslContext;
};


#endif // SSL_CONNECTION_HPP
