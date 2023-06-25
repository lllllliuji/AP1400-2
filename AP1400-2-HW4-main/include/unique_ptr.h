#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T>
class UniquePtr {
   public:
    UniquePtr(const UniquePtr<T> &) = delete;
    UniquePtr &operator=(const UniquePtr<T> &) = delete;
    UniquePtr() : _p(nullptr){};
    UniquePtr(T *ptr) : _p(ptr) {}
    ~UniquePtr();

    T &operator*();
    T *operator->();
    operator bool() const;

    void reset();
    void reset(T *);
    void reset(UniquePtr<T> &&rhs_ptr);
    T *release();
    T *get() { return _p; }

    friend UniquePtr make_unique(T data);

   private:
    T *_p;
};

#endif  // UNIQUE_PTR
#include "unique_ptr.hpp"