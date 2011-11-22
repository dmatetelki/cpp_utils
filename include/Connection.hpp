#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Socket.hpp"
#include "Message.hpp"

#include <string>

/** @todo make connection an iface and this class shall be a TcpConnection,
 * inherited from connection */

class Connection
{
public:

  Connection ( const int      socket,
               Message       *message,
               const size_t   bufferLength = 1024 );

  Connection ( const std::string   host,
               const std::string   port,
               Message            *message,
               const size_t        bufferLength = 1024 );

  virtual ~Connection();

  Connection* create(const int socket);

  bool connectToHost();
  bool bindToHost();
  bool listen( const int maxPendingQueueLen = 64 );
  void closeConnection();

  bool send( const void* message, const size_t length );
  bool receive();

  int getSocket() const;
  std::string getHost() const;
  std::string getPort() const;


private:

  Connection(const Connection&);
  Connection& operator=(const Connection&);

  Socket          m_socket;
  std::string     m_host;
  std::string     m_port;
  Message        *m_message;

  unsigned char  *m_buffer;
  size_t          m_bufferLength;
};


#endif // CONNECTION_HPP
