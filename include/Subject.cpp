#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <list>

class Observer;

class Subject
{
public:

  virtual ~Subject();

  /// @todo listen only to aspect?
  virtual void attach( Observer* );
  virtual void detach( Observer* );
  virtual void notify();

private:

  /// @todo list can be a priority queue
  std::list< Observer* > m_observers;

};

#endif // SUBJECT_HPP