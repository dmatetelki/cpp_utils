#ifndef SINGLETON_MEYERS_HPP
#define SINGLETON_MEYERS_HPP

template<typename T>
class Singleton_meyers
{
protected:

    Singleton_meyers() {};
    virtual ~Singleton_meyers() {};

private:

    Singleton_meyers( const Singleton_meyers& );
    Singleton_meyers& operator=( const Singleton_meyers& );

public:

    inline static T* getInstance()
    {
      /// @note static local initialization is thread safe in c++011
      static T instance;
      return &instance;
    }
};


#endif // SINGLETON_MEYERS_HPP
