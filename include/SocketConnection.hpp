#ifndef SOCKET_CONNECTION_HPP
#define SOCKET_CONNECTION_HPP

#include "Socket.hpp"
#include "Message.hpp"

#include <string>


class SocketConnection
{
public:

  SocketConnection ( const int      socket,
                     Message       *message,
                     const size_t   bufferLength = 1024 );

  SocketConnection ( const std::string   host,
                     const std::string   port,
                     Message            *message,
                     const size_t        bufferLength = 1024 );

  virtual ~SocketConnection();

  virtual SocketConnection* clone(const int socket) = 0;
  virtual bool connectToHost() = 0;
  virtual bool bindToHost() = 0;
  virtual bool listen( const int maxPendingQueueLen = 64 ) = 0;
  virtual void closeConnection() = 0;

  virtual bool send( const void* message, const size_t length ) = 0;
  virtual bool receive() = 0;

  int getSocket() const;
  std::string getHost() const;
  std::string getPort() const;

protected:

  Socket          m_socket;
  std::string     m_host;
  std::string     m_port;
  Message        *m_message;

  unsigned char  *m_buffer;
  size_t          m_bufferLength;

private:

  SocketConnection(const SocketConnection&);
  SocketConnection& operator=(const SocketConnection&);

};


#endif // SOCKET_CONNECTION_HPP
