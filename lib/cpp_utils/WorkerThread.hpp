#ifndef WORKER_THREAD_HPP
#define WORKER_THREAD_HPP

#include "Thread.hpp"
#include "ThreadPool.hpp"


class WorkerThread : public Thread
{

public:

  WorkerThread( ThreadPool& tp );

private:

  void* run();

  ThreadPool& m_tp;
};


#endif // WORKER_THREAD_HPP
