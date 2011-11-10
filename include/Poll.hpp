#ifndef POLL_HPP
#define POLL_HPP

#include <poll.h>

class Poll
{
public:

  Poll( const int socket, const nfds_t maxClient );
  virtual ~Poll();

  void startPolling();
  void stopPolling();

  virtual bool acceptClient();
  virtual bool handleClient( const int fd );
  bool receive( const int fd );

  bool addFd( const int fd, const short events );
  bool removeFd( const int fd );

private:

  int     m_socket;
  nfds_t  m_maxclients;
  pollfd *m_fds;
  nfds_t  m_num_of_fds;
  bool    m_polling;

};

#endif // POLL_HPP
