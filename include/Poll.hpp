#ifndef POLL_HPP
#define POLL_HPP

#include "Connection.hpp"

#include <poll.h>
#include <map>



class Poll
{
public:

  Poll( Connection   *connection,
        const nfds_t  maxClient = 10 );

  virtual ~Poll();

  void startPolling();
  void stopPolling();

  bool isPolling() const;


protected:

  // can be overriden: behaviour alters in server/client
  virtual void acceptClient();

  // can be overriden: behaviour alters in server/client
  virtual void handleClient( const int socket );


private:

  Poll(const Poll&);
  Poll& operator=(const Poll&);

  bool addFd( const int socket, const short events );
  bool removeFd( const int socket );


  typedef typename std::map< int, Connection* > ConnectionPool;

  Connection     *m_connection;
  volatile bool   m_polling;
  ConnectionPool  m_connectionPool;

  nfds_t          m_maxclients;
  pollfd         *m_fds;
  nfds_t          m_num_of_fds;

};

#endif // POLL_HPP
