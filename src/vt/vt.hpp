#ifndef VT_H
#define VT_H

#include <cstddef>
#include <initializer_list>
#include <span>
#include <stdexcept>

namespace vt {
template <class ValueType>
class Vector {
  using SizeType = std::size_t;
  using Reference = ValueType&;
  using ConstReference = const ValueType&;

public:
  Vector(std::initializer_list<ValueType> init)
      : capacity_(init.size())
      , logical_size_(init.size())
      , array_(new ValueType[this->capacity_], this->capacity_) {
    for (std::size_t i = 0; i < init.size(); i++) {
      this->array_[i] = init.begin()[i];
    }
  }

  Reference operator[](SizeType pos) {
    if (pos + 1 > this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return this->array_[pos];
  }

  ConstReference operator[](SizeType pos) const {
    if (pos + 1 > this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return this->array_[pos];
  }

  Reference At(SizeType pos) {
    if (pos + 1 > this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return this->array_[pos];
  }

  ConstReference At(SizeType pos) const {
    if (pos + 1 > this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return this->array_[pos];
  }

  Reference Front() {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->array_[0];
  }

  ConstReference Front() const {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->array_[0];
  }

  Reference Back() {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->array_[this->logical_size_ - 1];
  }

  ConstReference Back() const {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->array_[this->logical_size_ - 1];
  }

  [[nodiscard("Reason: Return value indicates if vector is empty")]] bool Empty() const {
    return this->logical_size_ == 0;
  }

  [[nodiscard("Reason: Return value shows current size of vector")]] SizeType Size() const {
    return this->logical_size_;
  }
  [[nodiscard("Reason: Return value shows current capacity of vector")]] SizeType Capacity() const {
    return this->capacity_;
  }

  void Clear() {
    this->logical_size_ = 0;
  }

  void PushBack(ConstReference value) {
    if (this->logical_size_ == this->capacity_) {
      if (this->capacity_ == 0) {
        std::span<ValueType> initial_block{new ValueType[2], 2};
        this->array_ = initial_block;
        this->capacity_ = 2;
      } else {
        this->capacity_ *= 2;
        std::span<ValueType> extended_block{new ValueType[this->capacity_], this->capacity_};
        for (SizeType i = 0; i < this->capacity_ / 2; i++) {
          extended_block[i] = this->array_[i];
        }
        this->array_ = extended_block;
      }
    }
    this->array_[this->logical_size_] = value;
    this->logical_size_++;
  }

  // void PushBack(ValueType&& value) {
  //   if (this->logical_size_ == this->capacity_) {
  //     if (this->capacity_ == 0) {
  //       std::span<ValueType> initial_block{new ValueType[2], 2};
  //       this->array_ = initial_block;
  //       this->capacity_ = 2;
  //     } else {
  //       this->capacity_ *= 2;
  //       std::span<ValueType, this->capacity_> extended_block;
  //       for (SizeType i = 0; i < this->capacity_ / 2; i++) {
  //         extended_block[i] = this->array_[i];
  //       }
  //       this->array_ = extended_block;
  //     }
  //   }
  //   this->array_[this->logical_size_] = value;
  //   this->logical_size_++;
  // }

  void PopBack() {
    if (Size() == 0) {
      return;
    }
    this->logical_size_--;
  }

private:
  SizeType logical_size_ = 0;
  SizeType capacity_ = 0;
  std::span<ValueType> array_ = nullptr;
};
}  // namespace vt

#endif