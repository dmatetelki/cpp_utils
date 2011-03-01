#include <iostream>

#ifndef NOTRACE
  #define TRACE(x) std::cout << x << " " << __PRETTY_FUNCTION__ <<  \
                          " : " << __LINE__ << std::endl

#else
  /// @todo Get rid of the "warning: statement has no effect" compiler msgs
  #define TRACE(x) ""
#endif


#include <assert.h>
#include <pthread.h>

#include <stdexcept>
#include <time.h>

class Mutex
{
public:

  Mutex() {
    TRACE(this);
    int ret = pthread_mutex_init( &m_mutex, 0 );
//     assert( ret == 0 );
  }
  ~Mutex() {
    TRACE(this);
    int ret = pthread_mutex_destroy ( &m_mutex );
//     assert( ret == 0 );
  }
  void lock() {
    TRACE(this);
    int ret = pthread_mutex_lock( &m_mutex );
//     assert( ret == 0 );
  }
  void unlock() {
    TRACE(this);
    int ret = pthread_mutex_unlock( &m_mutex );
//     assert( ret == 0 );
  }
// private:
  Mutex(const Mutex& m){
    TRACE(this);
  }
  Mutex& operator=(const Mutex& m) {
    TRACE(this);
    return *this;
  }

  int tryLock() {
    TRACE(this);
//     return pthread_mutex_trylock(&m_mutex);
    int interval = 1;

         timespec abs_time;
    clock_gettime ( CLOCK_REALTIME, &abs_time );
    abs_time.tv_nsec += interval * 1000000;
    if ( abs_time.tv_nsec >= 1000000000 ) {
      abs_time.tv_nsec -= 1000000000;
      abs_time.tv_sec += 1;
    }
    return pthread_mutex_timedlock ( &m_mutex, &abs_time );
  }

private:
  pthread_mutex_t m_mutex;
};


class BadScopedLock
{
public:

  BadScopedLock(Mutex& m)
  : m_mutex(m)  /// @note cctor called!
  {
    TRACE(this);
    m_mutex.lock();
  }
  ~BadScopedLock() {
    TRACE(this);
    m_mutex.unlock();
  }

private:
  BadScopedLock(const BadScopedLock&);
  BadScopedLock& operator=(const BadScopedLock&);

  Mutex& m_mutex;
};

class UnluckyUser
{
public:
  UnluckyUser() : m_mutex() {
    TRACE(this);
  }
  ~UnluckyUser() {
    TRACE(this);
  }
  void fv() {
    TRACE(this);
    BadScopedLock sl(m_mutex);
    throw std::logic_error("whoops");
  }
  int tryLock() {
    TRACE(this);
    return m_mutex.tryLock();
  }
private:
  Mutex m_mutex;
};

// class GoodScopedLock
// {
// public:
// 
//   GoodScopedLock(Mutex* m) : m_mutex(m)
//   {
//     TRACE(this);
//     m_mutex->lock();
//   }
//   ~GoodScopedLock() {
//     TRACE(this);
//     m_mutex->unlock();
//   }
// 
// private:
//   GoodScopedLock(const GoodScopedLock&);
//   GoodScopedLock& operator=(const GoodScopedLock&);
// 
//   Mutex* m_mutex;
// };
/*
class LuckyUser
{
public:
  LuckyUser() : m_mutex() {
    TRACE(this);
  }
  ~LuckyUser() {
    TRACE(this);
  }
  void fv() {
    TRACE(this);
    GoodScopedLock sl(&m_mutex);
    throw std::logic_error("whoops");
  }
  int tryLock() {
    TRACE(this);
    return m_mutex.tryLock();
  }
private:
  Mutex m_mutex;
};*/


int main()
{
  TRACE("main begin");
//   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// //   pthread_mutex_t mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
//   std::cout << "bla1" << std::endl;
//   std::cout << pthread_mutex_unlock( &mutex )<< std::endl;
//   std::cout << pthread_mutex_trylock( &mutex ) << std::endl;
//   std::cout << "bla2" << std::endl;
// //   std::cout << pthread_mutex_unlock( &mutex ) << std::endl;
//   
//   std::cout << "bla3" << std::endl;
// 
//     std::cout << pthread_mutex_lock( &mutex ) << std::endl;
//     std::cout << pthread_mutex_trylock( &mutex ) << std::endl;
// //       std::cout << pthread_mutex_lock( &mutex ) << std::endl;

  UnluckyUser u;
  try {
    u.fv();
  } catch (std::logic_error ex) {
    std::cout << "std::logicexception: " << ex.what() << std::endl;
    if (u.tryLock() == 0) {
      std::cout << "UnluckyUser: Ok, mutex is unlocked" << std::endl;
    } else {
      std::cout << "UnluckyUser: Failed, mutex is still locked" << std::endl;
    }
  }

//   TRACE("main middle");
// 
//   LuckyUser u2;
//   try {
//     u2.fv();
//   } catch (std::logic_error ex) {
//     std::cout << "std::logicexception: " << ex.what() << std::endl;
//     if (u2.tryLock() == 0) {
//       std::cout << "LuckyUser: Ok, mutex is unlocked" << std::endl;
//     } else {
//       std::cout << "LuckyUser: Failed, mutex is still locked" << std::endl;
//     }
//   }


  TRACE("main end");
  return 0;
}
