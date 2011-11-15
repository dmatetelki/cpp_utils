#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include "Socket.hpp"

#include <string>

class TcpConnection
{
public:

  enum Status {
    OPENED,
    CLOSED
  };

  TcpConnection ( const int socket,
                  const int bufferLenght = 1024 );

  TcpConnection ( const std::string host,
                  const std::string port,
                  const int bufferLenght = 1024 );

  virtual ~TcpConnection();

  bool connectToHost();
  bool bindToHost();

  void closeConnection();

  bool sendMessage( const void* message, const int length );
  bool readFromSocket();
  virtual void onMessageReady ( const unsigned char * message,
                                const int length ) = 0;


  int getSocket() const;


private:

  TcpConnection(const TcpConnection&);
  TcpConnection& operator=(const TcpConnection&);

  Socket          m_socket;
  std::string     m_host;
  std::string     m_port;
  Status          m_status;

  unsigned char  *m_buffer;
  int             m_bufferLength;
};


#endif // TCP_CONNECTION_HPP
