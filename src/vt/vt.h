#ifndef VT_H
#define VT_H

#include <string>

namespace vt {
  template<class value_type>
  class vector {
    typedef size_t size_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    public:
      vector(std::initializer_list<value_type> l);

      reference operator[](size_type pos);
      const_reference operator[](size_type pos) const;
      reference at(size_type pos);
      const_reference at(size_type pos) const;
      reference front(size_type pos);
      const_reference front(size_type pos) const;
      reference back(size_type pos);
      const_reference back(size_type pos) const;

      bool empty() const;
      size_type size() const;
      size_type capacity() const;

      void clear();
      void push_back(const_reference value);
      void push_back(value_type&& value);
      void pop_back();

    private:
      size_type _logical_size = 0;
      size_type _capacity = 0;
      value_type* _array;
  };
}

#include "vt.cpp"
#endif