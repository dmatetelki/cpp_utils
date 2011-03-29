#include <cxxtest/TestSuite.h>

// #include <time.h> // time

#include "Task.hpp"
#include "Thread.hpp"
#include "ThreadPool.hpp"
#include "Common.hpp"
#include "test_Common.hpp"



class TestThreadPoolSuite : public CxxTest::TestSuite
{

  class DummyTask : public Task
  {

  public:

    DummyTask() { /*m_timeOut = 5*/; TRACE; }

    void run()
    {
      TRACE;
//       m_startedToRun = time(NULL);
      LOG( Logger::FINEST, "I'm a task...");
//       m_startedToRun = 0;
    }

    bool isItStucked () const
    {
      TRACE;
      return false;
    }
  };

  class WorkerThread : public Thread
  {

  public:

    WorkerThread( ThreadPool& tp )
      : m_tp(tp)
    {
      TRACE;
    }

    private:

      void* run()
      {
        TRACE;
        while ( m_isRunning )
        {
          Task* task(0);
          try {
            task = m_tp.popTask();
            task->run();
            delete task;
          } catch (CancelledException) {
            LOG( Logger::FINEST, "Now I die.");
          }
        }
        return 0;
      }

      ThreadPool& m_tp;
  };


public:

  void testBasic()
  {
    TEST_HEADER;
    ThreadPool* tp = new ThreadPool();

    Thread* wt1 = new WorkerThread(*tp);
    Thread* wt2 = new WorkerThread(*tp);
    Thread* wt3 = new WorkerThread(*tp);
    tp->pushWorkerThread(wt1);
    tp->pushWorkerThread(wt2);
    tp->pushWorkerThread(wt3);
    tp->startWorkerThreads();

    Task* t1 = new DummyTask();
    tp->pushTask(t1);
    Task* t2 = new DummyTask();
    tp->pushTask(t2);

    sleep(2);

    tp->stop();
    tp->join();
    delete tp;
  }
};
