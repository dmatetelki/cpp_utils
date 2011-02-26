

#include <assert.h>
#include <pthread.h>

#include <stdexcept>

class Mutex
{
public:

  Mutex() : m_mutex(PTHREAD_MUTEX_INITIALIZER) {
    TRACE(this);
    int ret = pthread_mutex_init( &m_mutex, 0 );
    assert( ret == 0 );
  }
  ~Mutex() {
    TRACE(this);
    int ret = pthread_mutex_destroy ( &m_mutex );
    assert( ret == 0 );
  }
  void lock() {
    TRACE(this);
    int ret = pthread_mutex_lock( &m_mutex );
    assert( ret == 0 );
  }
  void unlock() {
    TRACE(this);
    int ret = pthread_mutex_unlock( &m_mutex );
    assert( ret == 0 );
  }
private:
  Mutex(const Mutex& m);
  Mutex& operator=(const Mutex& m);

private:
  pthread_mutex_t m_mutex;
};


class ScopedLock
{
public:

  ScopedLock(Mutex* m) : m_mutex(m) {
    TRACE(this);
    m_mutex->lock();
  }
  ~ScopedLock() {
    TRACE(this);
    m_mutex->unlock();
  }

private:
  ScopedLock(const ScopedLock&);
  ScopedLock& operator=(const ScopedLock&);

  Mutex* m_mutex;
};

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
  TRACE("main begin");

  User u;

  try {
    u.fv();
  } catch (std::logic_error ex) {
    std::cout << "std::logicexception: " << ex.what() << std::endl;
  }


  TRACE("main end");
  return 0;
}
