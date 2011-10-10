#ifndef SINGLETON_CALL_ONCE_HPP
#define SINGLETON_CALL_ONCE_HPP

#include <mutex>

template<typename T>
class Singleton_call_once
{
protected:

    Singleton_call_once() {};
    virtual ~Singleton_call_once() {};

private:

    Singleton_call_once( const Singleton_call_once& );
    Singleton_call_once& operator=( const Singleton_call_once& );

public:

    static T* getInstance()
    {
      std::call_once(m_flag, &Singleton_call_once::do_init);
      return m_instance;
    }

private:

    static void do_init()
    {
        m_instance = new T();
    }

    static T* m_instance;
    static std::once_flag m_flag;
};

template<class T> T* Singleton_call_once<T>::m_instance = 0;
template<class T> std::once_flag Singleton_call_once<T>::m_flag;

#endif // SINGLETON_CALL_ONCE_HPP
