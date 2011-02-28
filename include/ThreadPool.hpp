#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>

#include "ConcurrentQueue.hpp"
#include "WorkerThread.hpp"
#include "Task.hpp"
#include "Mutex.hpp"


class WorkerThread;

class ThreadPool
{

  public:

    ThreadPool( const int threadNum );
    virtual ~ThreadPool();

    void startWorkerThreads();

    virtual void pushTask( Task* task );
    Task* popTask();

    void stop();
    void join() const;

  private:

    ThreadPool( const ThreadPool& );
    ThreadPool& operator=( const ThreadPool& );

    int m_threadNum;
    std::vector<WorkerThread*> m_threads;
    ConcurrentQueue<Task*> m_tasks;
//     Mutex m_mutex;
};


#endif // THREADPOOL_HPP */
