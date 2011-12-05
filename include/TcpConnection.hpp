#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP


#include "StreamConnection.hpp"
#include "Message.hpp"
#include "Socket.hpp"

#include <string>


class TcpConnection : public StreamConnection
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

  Connection* clone(const int socket);

  bool connect();
  bool disconnect();

  bool send( const void* message, const size_t length );
  bool receive();

  int getSocket() const;

  bool bind();
  bool listen( const int maxPendingQueueLen = 64 );
  int accept();

private:

  TcpConnection(const TcpConnection&);
  TcpConnection& operator=(const TcpConnection&);

  Socket          m_socket;
  Message        *m_message;
  unsigned char  *m_buffer;
  size_t          m_bufferLength;
};


#endif // TCP_CONNECTION_HPP
