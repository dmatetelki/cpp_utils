#ifndef POLL_HPP
#define POLL_HPP

#include <poll.h>

class Poll
{
public:

  Poll( int &socket, const nfds_t maxClient );
  virtual ~Poll();

  void startPolling();
  void stopPolling();

  virtual void acceptClient();
  virtual void handleClient( const int fd );
  virtual bool receive( const int fd ) = 0;

  bool addFd( const int fd, const short events );
  bool removeFd( const int fd );


protected:

  bool m_polling;


private:

  Poll(const Poll&);
  Poll& operator=(const Poll&);

  int    &m_pollSocket;
  nfds_t  m_maxclients;
  pollfd *m_fds;
  nfds_t  m_num_of_fds;

};

#endif // POLL_HPP
