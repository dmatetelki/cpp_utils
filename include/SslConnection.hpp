#ifndef SSL_CONNECTION_HPP
#define SSL_CONNECTION_HPP


#include "StreamConnection.hpp"
#include "TcpConnection.hpp"
#include "Message.hpp"

#include <string>

// #include <openssl/rand.h>
#include <openssl/ssl.h>
// #include <openssl/err.h>



/// @note Call init/destroy before/after usage
class SslConnection : public StreamConnection
{
public:

  static void init();
  static void destroy();

  SslConnection ( const int      socket,
                  Message       *message,
                  const size_t   bufferLength = 1024 );

  SslConnection ( const std::string   host,
                  const int           port,
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
  int accept();

  int getSocket() const;

private:

  SslConnection(const SslConnection&);
  SslConnection& operator=(const SslConnection&);

  bool initHandle();
  std::string getSslError(const std::string &msg);
  bool loadCertificates( const std::string certificateFile,
                         const std::string keyFile );


  TcpConnection   m_tcpConnection;
  Message        *m_message;
  unsigned char  *m_buffer;
  size_t          m_bufferLength;
  SSL            *m_sslHandle;
  SSL_CTX        *m_sslContext;
};


#endif // SSL_CONNECTION_HPP
