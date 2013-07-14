#include "ThreadPool.hpp"
#include "Common.hpp"


ThreadPool::ThreadPool()
 : m_threads()
 , m_tasks()
{
  TRACE;
}

ThreadPool::~ThreadPool()
{
  TRACE;
  std::vector<Thread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    delete (*it);
  }
  m_threads.clear();
}

void ThreadPool::pushTask( Task* task )
{
  TRACE;
  m_tasks.push(task);
}


Task* ThreadPool::popTask()
{
  TRACE;
  return m_tasks.waitAndPop();
}


void ThreadPool::pushWorkerThread( Thread * thread)
{
  TRACE;
  m_threads.push_back( thread );
}


void ThreadPool::startWorkerThreads()
{
  TRACE;
  std::vector<Thread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ )
  {
    (*it)->start();
  }

}

void ThreadPool::stop()
{
  TRACE;
  std::vector<Thread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    (*it)->stop();
  }

  m_tasks.cancel();
}


void ThreadPool::join() const
{
  TRACE;
  std::vector<Thread*>::const_iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    (*it)->join();
  }
}
