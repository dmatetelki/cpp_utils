#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>

#include "ConcurrentDeque.hpp"
#include "Task.hpp"
#include "Thread.hpp"
#include "Mutex.hpp"


class ThreadPool
{

  public:

    ThreadPool();
    ~ThreadPool();

    void pushTask(Task* task);
    Task* popTask();

    void pushWorkerThread(Thread * thread);
    void startWorkerThreads();

    void stop();
    void join() const;

  private:

    ThreadPool(const ThreadPool&);
    ThreadPool& operator=(const ThreadPool&);

    std::vector<Thread*> m_threads;
    ConcurrentDeque<Task*> m_tasks;
};


#endif // THREADPOOL_HPP */
