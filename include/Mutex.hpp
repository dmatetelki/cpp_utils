#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

class Mutex
{

public:

  enum MutexType {
    Normal = PTHREAD_MUTEX_NORMAL, // no deadlock check, unlock a non-locked mutex results undefined behaviour
    ErrorCheck = PTHREAD_MUTEX_ERRORCHECK, // error returned when relock or unlocking a non-locked mutex
    Recursive = PTHREAD_MUTEX_RECURSIVE, // Lock counting with error handling.
    Default = PTHREAD_MUTEX_DEFAULT // equals normal
  };

  Mutex(MutexType type = MutexType::Default);
  ~Mutex();

  MutexType getType() const { return m_type; }
  pthread_mutex_t* getPThreadMutex() { return &m_mutex; }

  // If already locked, the calling thread shall block until the mutex becomes available.
  int lock();

  int unlock();

  // If currently locked, the call shall return immediately.
  int tryLock( const long int intervalSec = 0,
               const long int intervalNSec = 0 );

private:

  Mutex(const Mutex& m);
  Mutex& operator=(const Mutex& m);

  pthread_mutex_t m_mutex;
  MutexType m_type;
  pthread_mutexattr_t m_attr;
};

#endif // MUTEX_HPP

