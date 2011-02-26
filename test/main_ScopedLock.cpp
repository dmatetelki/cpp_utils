//  g++ -Wall -Wextra src/*.cpp test/main_Mutex.cpp -Iinclude -lpthread -lrt


#include "ScopedLock.hpp"
#include "Mutex.hpp"
#include "Common.hpp"

#include <stdexcept>

class User
{
public:
  User() : m_mutex() {
    TRACE(this);
  }
  ~User() {
    TRACE(this);
  }
  void fv() {
    TRACE(this);
    ScopedLock sl(&m_mutex);
    throw std::logic_error("whoops");
  }
private:
  Mutex m_mutex;
};

int main()
{
  User u;

  try {
    u.fv();
  } catch (std::logic_error ex) {
    std::cout << "std::logicexception: " << ex.what() << std::endl;
  }

  return 0;
}

