#pragma once

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
    Pool(Pool & other) = delete;
    Pool & operator=(const Pool&) = delete;

    class Object {
      public:
        Object(Pool *ptr_owner, std::shared_ptr<TType> ptr_self) :
          _ptrOwner(ptr_owner),
          _ptrSelf(std::move(ptr_self))
        {
        };
        TType *operator->() {
          return _ptr_self.get();
        };
        ~Object() {
          _ptrOwner->_release(_ptrSelf);
        };
      private:
        std::shared_ptr<TType> _ptrSelf;
        Pool *_ptrOwner;
    };

    void resize(const size_t &numberOfObjectStored) {
      _objs.resize(numberOfObjectStored);
      _size = numberOfObjectStored;
      _fill();
    };

    template<typename ... TArgs>
    Pool::Object<TType> acquire(TArgs&& p_args) {
      if (_objs.empty()) {
        throw std::runtime_error("Cannot acquire object from empty pool.")
      }
      std::shared_ptr<TType> ptr = _objs.back();
      _objs.pop_back();
      return ptr.get();
    };
    void _release(std::shared_ptr<TType> item);

    private:
      std::deque<std::shared_ptr<TType>> _objs;
      size_t _size;
      void _fill() {
        size_t len = _objs.size();
        if (len < _size) {
          size_t diff = _size - len;
          for (size_t i = 0; i < diff; i++) {
            _objs.push_back(std::make_shared<TType>());
          }
        }
      }
};

template <typename TType>
void Pool<TType>::_release(std::shared_ptr<TType> item) {
  _objs.push_back(std::move(item));
};