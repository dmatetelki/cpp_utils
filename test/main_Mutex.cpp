//  g++ -Wall -Wextra src/*.cpp test/main_Mutex.cpp -Iinclude -lpthread -lrt

#include <iostream>
#include <pthread.h>

#include "Mutex.hpp"

Mutex m;
int counter = 0;

void *functionC( void* params )
{
  m.lock();
  counter++;
  std::cout << "Counter value: " << counter << std::endl;
  m.unlock();
  return 0;
}


int main()
{
  pthread_t thread1, thread2;

  pthread_create( &thread1, 0, &functionC, 0 );
  pthread_create( &thread2, 0, &functionC, 0 );

  pthread_join( thread1, 0 );
  pthread_join( thread2, 0 );

  Mutex m2(PTHREAD_MUTEX_ERRORCHECK);

  m2.lock();
  m2.unlock();

  m2.unlock();
  m2.unlock();

    return 0;
}

