#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

class Mutex 
{
public:
	Mutex();
	~Mutex();

	void lock();
	void unlock();

private:

	pthread_mutex_t m_mutex;

};

#endif // MUTEX_HPP

