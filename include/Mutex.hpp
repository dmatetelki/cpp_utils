#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

class Mutex 
{

public:

  Mutex(int kind = PTHREAD_MUTEX_DEFAULT);
  ~Mutex();

  void lock();
  void unlock();
  bool tryLock(const int interval = 0);
  pthread_mutex_t* getPThreadMutex();


private:

  Mutex(const Mutex& m);
  Mutex& operator=(const Mutex& m);

  pthread_mutex_t m_mutex;

};

#endif // MUTEX_HPP

