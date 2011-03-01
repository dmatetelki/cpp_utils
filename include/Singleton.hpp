#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "Common.hpp"

template<typename T>
class Singleton
{
protected:

    Singleton() { TRACE("Simgleton::Singleton()"); }
    virtual ~Singleton() { TRACE("Simgleton::~Singleton()"); }

private:

    Singleton( const Singleton& );
    Singleton& operator=( const Singleton& );

public:

    static void createInstance()
    {
      TRACE("Simgleton::createInstance()");
      if ( not m_instance ) {
        m_instance = new T();
      }
    }


    static T* getInstance()
    {
      TRACE("Simgleton::getInstance()");
      return m_instance;
    }


    static void destroy()
    {
      TRACE("Simgleton::destroy()");
      if ( m_instance ) {
        delete m_instance;
      }
      m_instance = 0;
    }


private:

    static T* m_instance;
};

template<class T> T* Singleton<T>::m_instance = 0;

#endif // SINGLETON_HPP
