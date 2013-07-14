#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP


#include "StreamConnection.hpp"
#include "Message.hpp"
#include "Socket.hpp"

#include <string>


class TcpConnection : public StreamConnection
{
public:

  enum State {
    OPEN,
    CLOSED
  };


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

  bool bind();
  bool listen( const int maxPendingQueueLen = 64 );
  bool accept(int client_socket);

  int getSocket() const;
  void setState(const State state);

  bool closed() const;
  Message *getMessage() const;
  size_t getBufferLength() const;

private:

  TcpConnection ( const int      socket,
                  Message       *message,
                  const size_t   bufferLength = 1024 );

  TcpConnection(const TcpConnection&);
  TcpConnection& operator=(const TcpConnection&);

  Socket          m_socket;
  Message        *m_message;
  unsigned char  *m_buffer;
  size_t          m_bufferLength;
  State           m_state;
};


#endif // TCP_CONNECTION_HPP
