#pragma once

#include <cuchar>
#include <iostream>
#include <memory>
#include <deque>

/*
Manages a collection of reusable templated TType objects,
provided to the user via a Pool::Object class. This class
handles the acquirable pointer, and releases it back to the pool
when needed, calling the destructor of the TType object but
without deallocating the memory
*/

template <typename TType>
class Pool {
  public:
    Pool() : _size(0){};
    Pool(Pool & other) = delete; // for simplicity
    Pool & operator=(const Pool&) = delete; // for simplicity

    class Object {
      public:
        TType *operator->() {
          return *this;
        }
    };

    void resize(const size_t &numberOfObjectStored) {
      _objs.resize(numberOfObjectStored);
      _size = numberOfObjectStored;
    };

    template<typename ... TArgs>
    Pool::Object<TType> acquire(TArgs&& p_args) {
      std::shared_ptr new_obj = std::make_shared<TType>(p_args);
      _objs.push_back(new_obj);
      return new_obj;
    };

  private:
    std::deque<std::shared_ptr<TType>> _objs;
    size_t _size;
};