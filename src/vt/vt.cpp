#include "vt.h"

template<class value_type>
typename vt::vector<value_type>::reference vt::vector<value_type>::operator[](size_type pos) {
  if (pos + 1 > this->_logical_size) {
    throw std::out_of_range("index out of range");
  }

  return this->_array[pos];
}

template<class value_type>
typename vt::vector<value_type>::const_reference vt::vector<value_type>::operator[](size_type pos) const {
  if (pos + 1 > this->_logical_size) {
    throw std::out_of_range("index out of range");
  }

  return this->_array[pos];
}

template<class value_type>
typename vt::vector<value_type>::reference vt::vector<value_type>::at(size_type pos) {
  if (pos + 1 > this->_logical_size) {
    throw std::out_of_range("index out of range");
  }

  return this->_array[pos];
}

template<class value_type>
typename vt::vector<value_type>::const_reference vt::vector<value_type>::at(size_type pos) const {
  if (pos + 1 > this->_logical_size) {
    throw std::out_of_range("index out of range");
  }

  return this->_array[pos];
}

template<class value_type>
typename vt::vector<value_type>::reference vt::vector<value_type>::front() {
  if (empty()) {
    throw std::out_of_range("empty vector");
  }

  return this->_array[0];
}

template<class value_type>
typename vt::vector<value_type>::const_reference vt::vector<value_type>::front() const {
  if (empty()) {
    throw std::out_of_range("empty vector");
  }

  return this->_array[0];
}

template<class value_type>
typename vt::vector<value_type>::reference vt::vector<value_type>::back() {
  if (empty()) {
    throw std::out_of_range("empty vector");
  }

  return this->_array[this->_logical_size - 1];
}

template<class value_type>
typename vt::vector<value_type>::const_reference vt::vector<value_type>::back() const {
  if (empty()) {
    throw std::out_of_range("empty vector");
  }

  return this->_array[this->_logical_size - 1];
}

template<class value_type>
bool vt::vector<value_type>::empty() const {
  return this->_logical_size == 0;
}

template<class value_type>
typename vt::vector<value_type>::size_type vt::vector<value_type>::size() const {
  return this->_logical_size;
}

template<class value_type>
typename vt::vector<value_type>::size_type vt::vector<value_type>::capacity() const {
  return this->_capacity;
}

template<class value_type>
void vt::vector<value_type>::clear() {
  this->_logical_size = 0;
}

template<class value_type>
void vt::vector<value_type>::push_back(vt::vector<value_type>::const_reference value) {
  if (this->_logical_size == this->_capacity) {
    if (this->capacity == 0) {
      this->_array = new value_type[1];
      this->capacity = 1;
    } else {
      this->_capacity *= 2;
      value_type new_array[capacity()] = new value_type[capacity()];
      for (int i = 0; i < capacity() / 2; i++) {
        new_array[i] = this->_array[i];
      }
      this->_array = new_array;
    }
  }
  this->_array[this->_logical_size] = value;
  this->_logical_size++;
}

template<class value_type>
void vt::vector<value_type>::push_back(value_type&& value) {
  if (this->_logical_size == this->_capacity) {
    if (this->capacity == 0) {
      this->_array = new value_type[1];
      this->capacity = 1;
    } else {
      this->_capacity *= 2;
      value_type new_array[capacity()] = new value_type[capacity()];
      for (int i = 0; i < capacity() / 2; i++) {
        new_array[i] = this->_array[i];
      }
      this->_array = new_array;
    }
  }
  this->_array[this->_logical_size] = value;
  this->_logical_size++;
}

template<class value_type>
void vt::vector<value_type>::pop_back() {
  if (size() == 0) {
    return;
  }
  this->_logical_size--;
}

template<class T>
vt::vector<T>::vector(std::initializer_list<T> items) {
  this->_capacity = items.size();
  this->_logical_size = items.size();
  this->_array = new T[items.size()];
  for (size_t i = 0; i < items.size(); i++) {
    this->_array[i] = items.begin()[i];
  }
}