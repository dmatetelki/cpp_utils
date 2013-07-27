#ifndef POLL_HPP
#define POLL_HPP

#include "StreamConnection.hpp"

#include <poll.h>
#include <map>



class Poll
{
public:

  Poll( StreamConnection *connection,
        const nfds_t  maxClient = 10,
        const int timeOut = 10 * 1000 ); // 10sec

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

  typedef std::map< int, StreamConnection* > ConnectionMap;

  // can be overriden: behaviour alters in server/client
  virtual void removeTimeoutedConnections();

  ConnectionMap::iterator removeConnection(int socket, ConnectionMap::iterator it);

  bool addFd( const int socket, const short events );
  bool removeFd( const int socket );


  int                m_timeOut;
  StreamConnection  *m_connection;
  volatile bool      m_polling;
  ConnectionMap      m_connections;

  nfds_t             m_maxclients;
  pollfd            *m_fds;
  nfds_t             m_num_of_fds;

};

#endif // POLL_HPP
