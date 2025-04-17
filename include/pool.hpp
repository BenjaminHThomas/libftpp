#pragma once

#include <cuchar>
#include <iostream>
#include <memory>

/*
Manages a collection of reusable templated TType objects,
provided to the user via a Pool::Object class. This class
handles the acquirable pointer, and releases it back to the pool
when needed, calling the destructor of the TType object but
without deallocating the memory
*/

template <typename TType> class Pool {
private:
public:
  template <typename T> class Object {
  private:
  public:
    TType *operator->();
  };
  void resize(const size_t &numberOfObjectStored);
  template <typename... TArgs> Pool::Object<TType> acquire(TArgs &&...p_args);
};
