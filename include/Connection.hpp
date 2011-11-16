#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Logger.hpp"
#include "Common.hpp"

#include "Socket.hpp"

#include <string>


template <typename T>
class Connection
{
public:

  enum Status {
    OPENED,
    CLOSED
  };

  Connection ( const int    socket,
               const size_t bufferLength = 1024 )
    : m_socket(socket)
    , m_host()
    , m_port()
    , m_status(CLOSED)
    , m_message()
    , m_buffer(0)
    , m_bufferLength(bufferLength)

  {
    TRACE;

    m_socket.getPeerName(m_host, m_port);
    m_buffer = new unsigned char[m_bufferLength];
  }

  Connection ( const std::string host,
               const std::string port,
               const size_t      bufferLength = 1024 )
    : m_socket(AF_INET, SOCK_STREAM)
    , m_host(host)
    , m_port(port)
    , m_status(CLOSED)
    , m_message()
    , m_buffer(0)
    , m_bufferLength(bufferLength)
  {
    TRACE;
    m_socket.createSocket();
    m_buffer = new unsigned char[m_bufferLength];
  }

  virtual ~Connection()
  {
    TRACE;
    m_socket.closeSocket();
    delete[] m_buffer;
  }

  bool connectToHost()
  {
    TRACE;
    return m_socket.connectToHost(m_host, m_port);
  }

  bool bindToHost()
  {
    TRACE;
    return m_socket.bindToHost(m_host, m_port);
  }

  void closeConnection()
  {
    TRACE;
    m_socket.closeSocket();
  }

  bool send( const void* message, const size_t length )
  {
    TRACE;
    return m_socket.send( message, length );
  }

  bool receive()
  {
    TRACE;

    LOG ( Logger::DEBUG, std::string("receving on socket: ").
                          append(TToStr(m_socket.getSocket())).c_str() );

    ssize_t len = recv(m_socket.getSocket(), m_buffer, m_bufferLength, 0);

    LOG ( Logger::DEBUG, std::string("len: ").
                          append(TToStr(len)).append(" errno: ").
                          append(TToStr(errno)).c_str() );

    if (len == -1) {
      LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
      return false;
    }

    if (len == 0) {
      LOG( Logger::DEBUG, "Connection closed by peer." );
      return false;
    }

    return m_message.buildMessage( (void*)m_buffer, (size_t)len);
  }


  int getSocket() const
  {
    TRACE;
    return m_socket.getSocket();
  }


private:

  Connection(const Connection&);
  Connection& operator=(const Connection&);

  Socket          m_socket;
  std::string     m_host;
  std::string     m_port;
  Status          m_status;
  T               m_message;

  unsigned char  *m_buffer;
  size_t          m_bufferLength;
};


#endif // CONNECTION_HPP
