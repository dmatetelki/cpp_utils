#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <string>


class Connection
{
public:

  virtual ~Connection();
  virtual Connection* clone(const int socket) = 0;

  virtual bool bind() = 0;

  virtual bool send( const void* message, const size_t length ) = 0;
  virtual bool receive() = 0;

  std::string getHost() const;
  std::string getPort() const;

  void setHost(const std::string host);
  void setPort(const std::string port);

  virtual int getSocket() const = 0;


protected:

  Connection(const std::string host, const std::string port);

  std::string m_host;
  std::string m_port;


private:

  Connection(const Connection &);
  Connection& operator= (const Connection &);
};

#endif // CONNECTION_HPP
