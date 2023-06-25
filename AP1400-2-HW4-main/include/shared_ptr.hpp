#pragma once
#include "shared_ptr.h"
template <typename T>
SharedPtr<T> make_shared(T data) {
    return SharedPtr<T>(new T(data));
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    if (_p == nullptr && _count == nullptr) {
        return;
    }
    (*_count)--;
    if ((*_count) == 0) {
        delete _p;
        delete _count;
        _p = nullptr;
        _count = nullptr;
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &ptr) {
    (*ptr._count)++;
    _p = ptr._p;
    _count = ptr._count;
}

template <typename T>
T &SharedPtr<T>::operator*() {
    return *_p;
}

template <typename T>
T *SharedPtr<T>::operator->() {
    return _p;
}

template <typename T>
SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &sp) {
    if (this == &sp) {
        return *this;
    }
    (*_count)--;
    if (*_count == 0) {
        delete _p;
        delete _count;
    }
    _count = sp._count;
    _p = sp._p;
    (*_count)++;
    return *this;
}

template <typename T>
SharedPtr<T>::operator bool() const {
    return _p != nullptr;
}

template <typename T>
void SharedPtr<T>::reset() {
    (*_count)--;
    if (*_count == 0) {
        delete _p;
        delete _count;
        _p = nullptr;
        _count = nullptr;
        return;
    }
    _count = nullptr;
    _p = nullptr;
}

template <typename T>
void SharedPtr<T>::reset(T *ptr) {
    (*_count)--;
    if (*_count == 0) {
        delete _p;
    }
    (*_count) = 1;
    _p = ptr;
}