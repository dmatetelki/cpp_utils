#ifndef CONDITION_VARIABLE_HPP
#define CONDITION_VARIABLE_HPP

#include "Mutex.hpp"
#include <pthread.h>  // pthread_cond

class ConditionVariable
{

public:

  ConditionVariable(Mutex& mutex);
  ~ConditionVariable();

  int wait(const long int intervalSec = 0, const long int intervaNSec = 0);
  int signal();
  int broadcast();

private:

  ConditionVariable(const ConditionVariable&);
  ConditionVariable& operator=(const ConditionVariable&);

  Mutex& m_mutex;
  pthread_cond_t m_condVar;

};

#endif // CONDITION_VARIABLE_HPP
