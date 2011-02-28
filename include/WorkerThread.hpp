#ifndef WORKER_THREAD_HPP
#define WORKER_THREAD_HPP

#include "Thread.hpp"
#include "ThreadPool.hpp"

class ThreadPool;

class WorkerThread : public Thread
{

  public:

    WorkerThread( ThreadPool& tp );
    void stop();

  private:

    void* run();

    ThreadPool& m_tp;
    bool m_isRunning;

};

#endif // WORKER_THREAD_HPP
