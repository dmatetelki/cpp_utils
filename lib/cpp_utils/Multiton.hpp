#ifndef MULTITON_H
#define MULTITON_H

#include <map>

// http://stackoverflow.com/questions/2346091/c-templated-class-implementation-of-the-multiton-pattern

/// @note Not thread-safe! Use some concurrent map!

template <typename Key, typename T> class Multiton
{
public:

    static T& getRef( const Key& key )
    {
        return *getPtr(key);
    }

    static T* getPtr( const Key& key )
    {
        typename std::map<Key, T*>::const_iterator it = m_instances.find(key);

        if ( it != m_instances.end() ) {
            return (T*)(it->second);
        }

        T* instance = new T;
        m_instances[key] = instance;
        return instance;
    }

    static bool remove( const Key& key )
    {
        typename std::map<Key, T*>::const_iterator it = m_instances.find(key);

        if ( it == m_instances.end() ) {
            return false;
        }

        delete (*it).second;
        m_instances.remove(it);
        return true;
    }

    static void destroy()
    {
        typename std::map<Key, T*>::const_iterator it;
        for ( it = m_instances.begin(); it != m_instances.end(); ++it ) {
            delete (*it).second;
        }
        m_instances.clear();
    }

protected:

    Multiton() {}

    virtual ~Multiton() {}

private:
    Multiton(const Multiton&) {}
    Multiton& operator= (const Multiton&) { return *this; }

    static std::map<Key, T*> m_instances;
};

template <typename Key, typename T> std::map<Key, T*> Multiton<Key, T>::m_instances;

#endif // MULTITON_H
