#ifndef VT_H
#define VT_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace vt {
template <class ValueType>
class Vector {
  using SizeType = std::size_t;
  using Reference = ValueType&;
  using ConstReference = const ValueType&;

public:
  Vector(std::initializer_list<ValueType> init)
      : capacity_(init.size()), logical_size_(init.size()), array_(new ValueType[this->capacity_]) {
    for (std::size_t i = 0; i < init.size(); i++) {
      this->At(i) = init.begin()[i];
    }
  }

  // copy constructor
  Vector(Vector& other) {
    Vector(other.Data());
  }

  // move constructor
  Vector(Vector&& other) noexcept
      : array_(std::move(other.Data())), capacity_(other.Capacity()), logical_size_(other.Size()) {
  }

  // destructor
  ~Vector() {
    delete[] this->array_;
  }

  // copy assignment
  Vector& operator=(const Vector& other) {
    if (this == &other) {
      return *this;
    }
    this->capacity_ = other.Capacity();
    this->logical_size_ = other.Size();
    delete[] this->array_;
    auto copy_block = new ValueType[this->capacity_];
    this->array_ = copy_block;
    for (std::size_t i = 0; i < other.Size(); i++) {
      this->At(i) = other.Data().begin()[i];
    }
    return *this;
  }

  // move assignment
  Vector& operator=(Vector&& other) noexcept {
    this->capacity_ = other.Capacity();
    this->logical_size_ = other.Size();
    delete[] this->array_;
    this->array_ = std::move(other.Data());
    return *this;
  }

  Reference operator[](SizeType pos) {
    return this->At(pos);
  }

  ConstReference operator[](SizeType pos) const {
    return this->At(pos);
  }

  Reference At(SizeType pos) {
    if (pos >= this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return *(this->array_ + pos);
  }

  ConstReference At(SizeType pos) const {
    if (pos >= this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return *(this->array_ + pos);
  }

  Reference Front() {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(0);
  }

  ConstReference Front() const {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(0);
  }

  Reference Back() {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(this->logical_size_ - 1);
  }

  ConstReference Back() const {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(this->logical_size_ - 1);
  }

  ValueType* Data() {
    return this->array_;
  }

  const ValueType* Data() const {
    return this->array_;
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
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto initial_block = new ValueType[2];
        delete[] this->array_;
        this->array_ = initial_block;
        this->capacity_ = 2;
      } else {
        this->capacity_ *= 2;
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        auto extended_block = new ValueType[this->capacity_];
        for (SizeType i = 0; i < this->capacity_ / 2; i++) {
          extended_block[i] = this->At(i);
        }
        delete[] this->array_;
        this->array_ = extended_block;
      }
    }
    this->logical_size_++;
    this->At(this->logical_size_ - 1) = value;
  }

  void PopBack() {
    if (Size() == 0) {
      return;
    }
    this->logical_size_--;
  }

private:
  SizeType logical_size_ = 0;
  SizeType capacity_ = 0;
  ValueType* array_ = nullptr;
};
}  // namespace vt

#endif