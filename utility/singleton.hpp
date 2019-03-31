#pragma once

template <class T>
class Singleton
{
public:
    Singleton(T& )
    { }

    ~Singleton()
    {
        s_pInstance = nullptr;
    }

    static T* i()
    {
        if (s_pInstance == nullptr) {
            s_pInstance = new T();
        }
        return (s_pInstance);
    }

private:
    static T* s_pInstance;

    Singleton(const Singleton& src);
    Singleton& operator=(const Singleton& src);
};

template <class T> T* Singleton<T>::s_pInstance = nullptr;
