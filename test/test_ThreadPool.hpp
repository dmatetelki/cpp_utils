#include <cxxtest/TestSuite.h>

#include "Task.hpp"
#include "Thread.hpp"
#include "WorkerThread.hpp"
#include "ThreadPool.hpp"
#include "Common.hpp"
#include "Fixture.hpp"



class TestThreadPoolSuite : public CxxTest::TestSuite
{

  class DummyTask : public Task
  {

  public:

    DummyTask()
    {
      TRACE;
    }

    void run()
    {
      TRACE;
      LOG( Logger::FINEST, "I'm a task...");
    }

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
