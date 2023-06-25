#pragma once
#include "unique_ptr.h"
template <typename T>
UniquePtr<T> make_unique(T data) {
    return UniquePtr<T>(new T(data));
}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    delete _p;
    _p = nullptr;
}

template <typename T>
T &UniquePtr<T>::operator*() {
    return *_p;
}

template <typename T>
T *UniquePtr<T>::operator->() {
    return _p;
}

template <typename T>
UniquePtr<T>::operator bool() const {
    return _p != nullptr;
}

template <typename T>
void UniquePtr<T>::reset() {
    delete _p;
    _p = nullptr;
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) {
    delete _p;
    _p = ptr;
}

template <typename T>
void UniquePtr<T>::reset(UniquePtr<T> &&rhs_ptr) {
    delete _p;
    _p = rhs_ptr._p;
    rhs_ptr._p = nullptr;
}

template <typename T>
T *UniquePtr<T>::release() {
    auto p = _p;
    _p = nullptr;
    return p;
}