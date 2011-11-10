#ifndef POLL_HPP
#define POLL_HPP

#include <poll.h>

class Poll
{
public:

  Poll( int &socket, const nfds_t maxClient );
  virtual ~Poll();

  void setOwnSocket( const int socket );
  void startPolling();
  void stopPolling();

  virtual void acceptClient();
  virtual void handleClient( const int socket );
  virtual bool receive( const int socket ) = 0;


protected:

  bool m_polling;


private:

  Poll(const Poll&);
  Poll& operator=(const Poll&);

  bool addFd( const int socket, const short events );
  bool removeFd( const int socket );

  int    &m_pollSocket;
  nfds_t  m_maxclients;
  pollfd *m_fds;
  nfds_t  m_num_of_fds;

};

#endif // POLL_HPP
