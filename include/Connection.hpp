#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Logger.hpp"
#include "Common.hpp"

#include "Socket.hpp"

#include <string>

/** @todo make connection an iface and this class shall be a TcpConnection,
 * inherited from connection */

template <typename T>
class Connection
{
public:

  enum Status {
    OPENED,
    CLOSED
  };

  Connection ( const int      socket,
               void          *msgParam = 0,
               const size_t   bufferLength = 1024 )
    : m_socket(socket)
    , m_host()
    , m_port()
    , m_status(CLOSED)
    , m_message(this, msgParam)
    , m_buffer(0)
    , m_bufferLength(bufferLength)

  {
    TRACE;

    m_socket.getPeerName(m_host, m_port);
    m_buffer = new unsigned char[m_bufferLength];
  }

  Connection ( const std::string   host,
               const std::string   port,
               void               *msgParam = 0,
               const size_t        bufferLength = 1024 )
    : m_socket(AF_INET, SOCK_STREAM)
    , m_host(host)
    , m_port(port)
    , m_status(CLOSED)
    , m_message(this, msgParam)
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

  bool listen( const int maxPendingQueueLen = 64 )
  {
    TRACE;
    return m_socket.listen( maxPendingQueueLen );
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

    ssize_t length;
    if ( !m_socket.receive(m_buffer, m_bufferLength, &length) ) {
      if (length == -1) {
        LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
      }
      else if (length == 0) {
        LOG( Logger::INFO, std::string("Connection closed by ").
                        append(m_host).append(":").append(m_port).c_str() );
      }
    return false;
    }

    LOG ( Logger::DEBUG, std::string("Received: ").
                        append(TToStr(length)).append(" bytes from: ").
                        append(m_host).append(":").append(m_port).c_str() );

    return m_message.buildMessage( (void*)m_buffer, (size_t)length);
  }


  int getSocket() const
  {
    TRACE;
    return m_socket.getSocket();
  }

  std::string getHost() const
  {
    TRACE;
    return m_host;
  }

  std::string getPort() const
  {
    TRACE;
    return m_port;
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
