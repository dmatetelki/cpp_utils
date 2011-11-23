#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP


#include "SocketConnection.hpp"
#include "Message.hpp"

#include <string>


class TcpConnection : public SocketConnection
{
public:

  TcpConnection ( const int      socket,
                  Message       *message,
                  const size_t   bufferLength = 1024 );

  TcpConnection ( const std::string   host,
                  const std::string   port,
                  Message            *message,
                  const size_t        bufferLength = 1024 );

  virtual ~TcpConnection();

  SocketConnection* clone(const int socket);

  bool connectToHost();
  bool bindToHost();
  bool listen( const int maxPendingQueueLen = 64 );
  void closeConnection();

  bool send( const void* message, const size_t length );
  bool receive();


private:

  TcpConnection(const TcpConnection&);
  TcpConnection& operator=(const TcpConnection&);
};


#endif // TCP_CONNECTION_HPP
