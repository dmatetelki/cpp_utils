#ifndef STREAM_CONNECTION_HPP
#define STREAM_CONNECTION_HPP


#include "Connection.hpp"

#include "string"

class StreamConnection : public Connection
{
public:

  virtual ~StreamConnection() {};
  virtual Connection* clone(const int socket) = 0;

  virtual bool connect() = 0;
  virtual bool disconnect() = 0;

  virtual bool listen( const int maxPendingQueueLen = 64 ) = 0;

  /// @todo move accept and poll here
  virtual int accept() = 0;
//   virtual bool poll() = 0;


protected:

  StreamConnection(std::string host = std::string("invalid"), int port = -1)
   : Connection(host, port) {};


private:

  StreamConnection(const StreamConnection &);
  StreamConnection& operator= (const StreamConnection &);
};

#endif // STREAM_CONNECTION_HPP
