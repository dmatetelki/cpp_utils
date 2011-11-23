#ifndef SSL_CONNECTION_HPP
#define SSL_CONNECTION_HPP


#include "SocketConnection.hpp"
#include "TcpConnection.hpp"
#include "Message.hpp"

#include <string>

// #include <openssl/rand.h>
#include <openssl/ssl.h>
// #include <openssl/err.h>



/// @note Call init/destroy before/after usage
class SslConnection : public SocketConnection
{
public:

  static void init();
  static void destroy();

  SslConnection ( const int      socket,
                  Message       *message,
                  const size_t   bufferLength = 1024 );

  SslConnection ( const std::string   host,
                  const std::string   port,
                  Message            *message,
                  const size_t        bufferLength = 1024 );

  virtual ~SslConnection();

  SocketConnection* clone(const int socket);

  bool connectToHost();
  bool bindToHost();
  bool listen( const int maxPendingQueueLen = 64 );
  void closeConnection();

  bool send( const void* message, const size_t length );
  bool receive();


private:

  SslConnection(const SslConnection&);
  SslConnection& operator=(const SslConnection&);

  bool connect();
  std::string getSslError(const std::string &msg);


  TcpConnection   m_tcpConnection;
  SSL            *m_sslHandle;
  SSL_CTX        *m_sslContext;
};


#endif // SSL_CONNECTION_HPP
