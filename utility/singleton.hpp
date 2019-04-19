#pragma once

template <class T>
class Singleton {
public:
    Singleton(T& ) {}

    ~Singleton() {
        _pInstance = nullptr;
    }

    static T* i() {
        if (_pInstance == nullptr) {
            _pInstance = new T();
        }
        
        return (_pInstance);
    }

private:
    static T* _pInstance;

    Singleton(const Singleton& src);
    Singleton& operator=(const Singleton& src);
};

template <class T> T* Singleton<T>::_pInstance = nullptr;
