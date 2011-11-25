#include "Connection.hpp"

#include "Logger.hpp"


Connection::Connection(std::string host, int port)
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


int Connection::getPort() const
{
  TRACE;
  return m_port;
}


void Connection::setHost(const std::string host)
{
  TRACE;
  m_host = host;
}


void Connection::setPort(const int port)
{
  TRACE;
  m_port = port;
}


