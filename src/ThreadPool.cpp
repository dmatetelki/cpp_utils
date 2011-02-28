#include "ThreadPool.hpp"
#include "Common.hpp"


ThreadPool::ThreadPool( const int threadNum ) : m_threadNum( threadNum )
{
  TRACE(this);
}

ThreadPool::~ThreadPool()
{
  TRACE(this);
  std::vector<WorkerThread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    delete (*it);
  }
  m_threads.clear();
}

void ThreadPool::pushTask( Task* task )
{
  TRACE(this);
  m_tasks.push(task);
}

Task* ThreadPool::popTask()
{
  TRACE(this);
  return m_tasks.waitAndPop();
}


void ThreadPool::startWorkerThreads()
{
  TRACE(this);
  for( int i = 0; i<m_threadNum; i++ ) 
  {
    WorkerThread* t = new WorkerThread ( *this );
    m_threads.push_back( t );
    t->start();
  }
}

void ThreadPool::stop()
{
  TRACE(this);
  std::vector<WorkerThread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    (*it)->stop();
  }

  m_tasks.cancel();
}


void ThreadPool::join() const
{
  TRACE(this);
  std::vector<WorkerThread*>::const_iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    (*it)->join();
  }
}
