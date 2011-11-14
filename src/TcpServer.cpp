#include "TcpServer.hpp"

#include "Logger.hpp"
#include "Common.hpp"

#include "MessageBuilder.hpp"


TcpServer::TcpServer( const std::string host,
                      const std::string port,
                      const int maxClients )
  : Socket(AF_INET, SOCK_STREAM)
  , Poll(m_socket, maxClients)
  , m_host(host)
  , m_port(port)
{
  TRACE;
}


TcpServer::~TcpServer()
{
  TRACE;
}


bool TcpServer::start()
{
  TRACE;

  if ( !openSocket() )
    return false;

//   // Set the socket REUSABLE flag for the quick restart ability.
//   if (setsockopt(m_server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
//   {
//     errorArrived( EC_SETSOCKOPT ) ;
//   }


//   // Set the socket NONBLOCKING flag for polling.
//   if (-1 == (fc_flags = fcntl(m_server_socket, F_GETFL, 0)))
//   {
//     fc_flags = 0;
//   }
//   fcntl(m_server_socket, F_SETFL, fc_flags | O_NONBLOCK);


  if ( !bindToHost(m_host, m_port) )
    return false;

  if ( listen(m_socket, 64) == -1 ) {
    LOG( Logger::ERR, errnoToString("ERROR listening. ").c_str() );
    return false;
  }

  setOwnSocket(m_socket);
  startPolling();

  return true;
}


void TcpServer::stop()
{
  TRACE;

  stopPolling();
  closeSocket();
}


bool TcpServer::receive(const int clientSocket)
{
  TRACE;

  unsigned char buffer[10];
  int len = recv( clientSocket, buffer , 10, 0) ;

  if (len == -1) {
    LOG( Logger::ERR, errnoToString("ERROR reading from socket. ").c_str() );
    return false;
  }

  if (len == 0) {
    LOG( Logger::DEBUG, "Connection closed by peer." );
    return false;
  }

  MessageBuilder *m_builder(0);

  if ( !m_builder ) {
    msgArrived(clientSocket, buffer, len);
    return true;
  }

  return m_builder->buildMessage(buffer, len);

  return true;
}
