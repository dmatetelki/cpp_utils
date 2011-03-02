#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>

#include "ConcurrentQueue.hpp"
#include "Task.hpp"
#include "Thread.hpp"
#include "Mutex.hpp"


class ThreadPool
{

  public:

    ThreadPool( const int threadNum );
    ~ThreadPool();

    void pushTask( Task* task );
    Task* popTask();

    void pushWorkerThread( Thread * thread);
    void startWorkerThreads();

    void stop();
    void join() const;

  private:

    ThreadPool( const ThreadPool& );
    ThreadPool& operator=( const ThreadPool& );

    int m_threadNum;
    std::vector<Thread*> m_threads;
    ConcurrentQueue<Task*> m_tasks;
//     Mutex m_mutex;
};


#endif // THREADPOOL_HPP */
