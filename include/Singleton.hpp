#ifndef SINGLETON_HPP
#define SINGLETON_HPP


template<typename T>
class Singleton
{
protected:

    Singleton() {};
    virtual ~Singleton() {};

private:

    Singleton( const Singleton& );
    Singleton& operator=( const Singleton& );

public:

    static void createInstance()
    {
      if ( not m_instance ) {
        m_instance = new T();
      }
    }


    inline static T* getInstance()
    {
      return m_instance;
    }


    static void destroy()
    {
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
