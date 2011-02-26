#ifndef SCOPED_LOCK_HPP
#define SCOPED_LOCK_HPP

#include "Mutex.hpp"

class ScopedLock
{

public:

  ScopedLock(Mutex* m);
  ~ScopedLock();

private:

  ScopedLock(const ScopedLock&);
  ScopedLock& operator=(const ScopedLock&);

  Mutex* m_mutex;

};

#endif // SCOPED_LOCK_HPP
