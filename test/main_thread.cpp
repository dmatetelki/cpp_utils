#include <iostream>

#include <signal.h> // signal to thread
#include <stdlib.h> // malloc
// #include <string.h>

#include "Thread.hpp"
#include "Common.hpp"

class ThreadClass : public Thread
{
public:

  ~ThreadClass() { TRACE(this); }
  ThreadClass() { TRACE(this); }

private:

  void* run( void ) {
    TRACE(this);

    void *retval = malloc(sizeof(int));
    *((int*)retval) = 14;
    return retval;
    return 0;
  }
};

class ThreadClassWithSignal : public Thread
{
public:

  ~ThreadClassWithSignal() {
    TRACE(this);
  }
  ThreadClassWithSignal() {
    TRACE(this);
    signal(SIGINT, signal_handler);
  }

private:

  void* run( void ) {
    TRACE(this);

    void *retval = malloc(sizeof(int));
    *((int*)retval) = 14;
    return retval;
    return 0;
  }

  void static signal_handler(int sig)
  {
    TRACE("ThreadClassWithSignal::signal_handler");
    if (sig==SIGINT) {
      TRACE("signal_handler got SIGINT");
      pthread_exit(0);
    }
  }
};


int main()
{
  TRACE("main start");

  ThreadClass *m = new ThreadClass;
  m->start();

  void *retVal = m->join();
  std::cout << "got retVal: " << *((int*)retVal) << std::endl;
  free(retVal);
  delete m;

  ThreadClass *m2 = new ThreadClass;
  m2->start();
  m2->sendSignal(SIGINT);

  sleep(3);

  std::cout << "after sendSignal the:" << m2 <<std::endl;
  delete m2;


  TRACE("main end");
  return 0;
}
