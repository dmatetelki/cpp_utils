#ifndef SINGLETON_DCLP_HPP
#define SINGLETON_DCLP_HPP

#include <mutex>


template<typename T>
class Singleton_DCLP
{
protected:

    Singleton_DCLP() {};
    virtual ~Singleton_DCLP() {};

private:

    Singleton_DCLP( const Singleton_DCLP& );
    Singleton_DCLP& operator=( const Singleton_DCLP& );

public:

    static T* getInstance()
    {
      if ( not m_instance )
      {
        std::lock_guard<std::mutex> guard(m_lock);

        // this is now the critical section

        if ( not m_instance ) // re-check pinstance
        {
          // Douglas Schmidt proposed volatile
          // to prevent "agressive optimalizations"
          volatile T *temp = new T();
          m_instance = (T*)temp;
        }
      }

      return m_instance;
    }

    static void destroy()
    {
      delete m_instance;
    }


private:

    static std::mutex m_lock;
    static T* m_instance;
};

template<class T> std::mutex Singleton_DCLP<T>::m_lock;
template<class T> T* Singleton_DCLP<T>::m_instance = 0;


#endif // SINGLETON_DCLP_HPP
