#include "Connection.hpp"

#include "Logger.hpp"


Connection::Connection(const std::string host, const std::string port)
  : m_host(host)
  , m_port(port)
{
  TRACE;
}


Connection::~Connection()
{
  TRACE;
}


std::string Connection::getHost() const
{
  TRACE;
  return m_host;
}


std::string Connection::getPort() const
{
  TRACE;
  return m_port;
}


void Connection::setHost(const std::string host)
{
  TRACE;
  m_host = host;
}


void Connection::setPort(const std::string port)
{
  TRACE;
  m_port = port;
}


