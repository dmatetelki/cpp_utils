#ifndef ADDR_INFO_HPP
#define ADDR_INFO_HPP

#include <netdb.h>
#include <string>


class AddrInfo
{
public:

  AddrInfo();
  virtual ~AddrInfo();

  bool getHostInfo(const std::string host, const std::string port);
  addrinfo* operator[](const unsigned int pos);
  void printHostDetails() const;

  static bool convertNameInfo(const addrinfo* addrInfo,
                              std::string &retAddr,
                              std::string &retService);

private:

  AddrInfo(const AddrInfo&);
  AddrInfo& operator=(const AddrInfo&);

  struct addrinfo *m_addrInfo;
};

#endif // ADDR_INFO_HPP
