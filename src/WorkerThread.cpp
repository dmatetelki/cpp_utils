#include "WorkerThread.hpp"
#include "Task.hpp"
#include "Common.hpp"


WorkerThread::WorkerThread( ThreadPool& tp )
  : m_tp(tp)
  , m_isRunning(true)
{
  TRACE(this);
}


void WorkerThread::stop()
{
  TRACE(this);
  m_isRunning = false;
}


void* WorkerThread::run()
{
  TRACE(this);
  while ( m_isRunning )
  {
    Task* task(0);
    try {
      task = m_tp.popTask();
      task->run();
      delete task;
    } catch (CancelledException) {
      std::cout << "Now I die." << std::endl;
    }

  }

  return 0;
}


