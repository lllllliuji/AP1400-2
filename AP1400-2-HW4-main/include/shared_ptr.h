#ifndef SHARED_PTR
#define SHARED_PTR
#include <iostream>
template <typename T>
class SharedPtr {
   public:
    SharedPtr() : _p(nullptr) { _count = new int{0}; }
    SharedPtr(T *ptr) : _p(ptr) { _count = new int{1}; }
    SharedPtr(const SharedPtr<T> &);
    ~SharedPtr();

    T &operator*();
    T *operator->();
    SharedPtr<T> &operator=(const SharedPtr<T> &);
    operator bool() const;

    void reset();
    void reset(T *);
    void reset(SharedPtr<T> &&rhs_ptr);
    T *release();
    T *get() { return _p; }
    int use_count() { return _count == nullptr ? 0 : *_count; }

    friend SharedPtr<T> make_shared(T);

   private:
    T *_p;
    int *_count;
};

#endif  // SHARED_PTR

#include "shared_ptr.hpp"