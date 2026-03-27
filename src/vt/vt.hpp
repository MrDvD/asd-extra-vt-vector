#ifndef VT_H
#define VT_H

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>

namespace vt {
template <class ValueType>
class IteratorImpl {
public:
  using DifferenceType = std::ptrdiff_t;
  using Pointer = ValueType*;
  using Reference = ValueType&;
  using ConstReference = const ValueType&;
  // below is for iterator_traits
  // NOLINTBEGIN(readability-identifier-naming)
  using difference_type = DifferenceType;
  using value_type = std::remove_const_t<ValueType>;
  using reference = Reference;
  using const_reference = ConstReference;
  using iterator_concept = std::contiguous_iterator_tag;
  // NOLINTEND(readability-identifier-naming)

  IteratorImpl() = default;

  explicit IteratorImpl(Pointer ptr) : ptr_(ptr) {
  }

  Reference operator*() const {
    return *(this->ptr_);
  }

  IteratorImpl& operator++() {
    this->ptr_ = std::next(this->ptr_);
    return *this;
  }

  IteratorImpl operator++(int) {
    IteratorImpl temp = *this;
    ++(*this);
    return temp;
  }

  IteratorImpl& operator--() {
    this->ptr_ = std::prev(this->ptr_);
    return *this;
  }

  IteratorImpl operator--(int) {
    IteratorImpl temp = *this;
    --(*this);
    return temp;
  }

  bool operator<(const IteratorImpl& other) const {
    return this->ptr_ < other.ptr_;
  }

  bool operator<=(const IteratorImpl& other) const {
    return this->ptr_ <= other.ptr_;
  }

  bool operator>(const IteratorImpl& other) const {
    return this->ptr_ > other.ptr_;
  }

  bool operator>=(const IteratorImpl& other) const {
    return this->ptr_ >= other.ptr_;
  }

  bool operator==(const IteratorImpl& other) const {
    return this->ptr_ == other.ptr_;
  }

  DifferenceType operator-(const IteratorImpl& other) const {
    return this->ptr_ - other.ptr_;
  }

  IteratorImpl& operator+=(const DifferenceType diff) {
    this->ptr_ += diff;
    return this;
  }

  IteratorImpl operator+(DifferenceType diff) const {
    return IteratorImpl(this->ptr_ + diff);
  }

  IteratorImpl friend operator+(DifferenceType diff, const IteratorImpl& iter) {
    return iter + diff;
  }

  IteratorImpl& operator-=(const DifferenceType diff) {
    this->ptr_ -= diff;
    return this;
  }

  IteratorImpl operator-(DifferenceType diff) const {
    return IteratorImpl(this->ptr_ - diff);
  }

  IteratorImpl friend operator-(DifferenceType diff, const IteratorImpl& iter) {
    return iter - diff;
  }

  Reference operator[](DifferenceType diff) const {
    return *(this->ptr_ + diff);
  }

  Pointer operator->() const {
    return this->ptr_;
  }

private:
  Pointer ptr_ = nullptr;
};

template <class ValueType>
class Vector {
  using SizeType = std::size_t;
  using DifferenceType = std::ptrdiff_t;
  using Reference = ValueType&;
  using Pointer = ValueType*;
  using ConstReference = const ValueType&;
  using Iterator = IteratorImpl<ValueType>;
  using ConstIterator = IteratorImpl<const ValueType>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  static_assert(std::contiguous_iterator<Iterator>);
  static_assert(std::contiguous_iterator<ConstIterator>);

public:
  Vector(std::initializer_list<ValueType> init)
      : capacity_(init.size()), logical_size_(init.size()), array_(new ValueType[this->capacity_]) {
    for (std::size_t i = 0; i < init.size(); i++) {
      this->At(i) = init.begin()[i];
    }
  }

  // copy constructor
  constexpr Vector(Vector& other) {
    Vector(other.Data());
  }

  // move constructor
  constexpr Vector(Vector&& other) noexcept
      : array_(std::move(other.Data())), capacity_(other.Capacity()), logical_size_(other.Size()) {
  }

  // destructor
  constexpr ~Vector() {
    delete[] this->array_;
  }

  // copy assignment
  constexpr Vector& operator=(const Vector& other) {
    if (this == &other) {
      return *this;
    }
    this->capacity_ = other.Capacity();
    this->logical_size_ = other.Size();
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    auto copy_block = new ValueType[this->capacity_];
    delete[] this->array_;
    this->array_ = copy_block;
    for (std::size_t i = 0; i < other.Size(); i++) {
      this->At(i) = other.Data().begin()[i];
    }
    return *this;
  }

  // move assignment
  constexpr Vector& operator=(Vector&& other) noexcept {
    this->capacity_ = other.Capacity();
    this->logical_size_ = other.Size();
    delete[] this->array_;
    this->array_ = std::move(other.Data());
    return *this;
  }

  constexpr Reference At(SizeType pos) {
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

  constexpr Reference operator[](SizeType pos) {
    return this->At(pos);
  }

  constexpr ConstReference operator[](SizeType pos) const {
    return this->At(pos);
  }

  constexpr Reference Front() {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(0);
  }

  constexpr ConstReference Front() const {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(0);
  }

  constexpr Reference Back() {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(this->logical_size_ - 1);
  }

  constexpr ConstReference Back() const {
    if (Empty()) {
      throw std::out_of_range("empty vector");
    }

    return this->At(this->logical_size_ - 1);
  }

  constexpr ValueType* Data() noexcept {
    return this->array_;
  }

  constexpr const ValueType* Data() const noexcept {
    return this->array_;
  }

  constexpr Iterator Begin() noexcept {
    if (Empty()) {
      return IteratorImpl<ValueType>();
    }
    return IteratorImpl<ValueType>(this->array_);
  }

  constexpr ConstIterator Begin() const noexcept {
    if (Empty()) {
      return IteratorImpl<const ValueType>();
    }
    return IteratorImpl<const ValueType>(this->array_);
  }

  constexpr Iterator End() noexcept {
    if (Empty()) {
      return IteratorImpl<ValueType>();
    }
    return IteratorImpl<ValueType>(this->array_ + this->logical_size_);
  }

  constexpr ConstIterator End() const noexcept {
    if (Empty()) {
      return IteratorImpl<const ValueType>();
    }
    return IteratorImpl<const ValueType>(this->array_ + this->logical_size_);
  }

  constexpr ReverseIterator RBegin() noexcept {
    if (Empty()) {
      return std::reverse_iterator(IteratorImpl<ValueType>());
    }
    return std::reverse_iterator(IteratorImpl<ValueType>(this->array_ + this->logical_size_));
  }

  constexpr ConstReverseIterator RBegin() const noexcept {
    if (Empty()) {
      return std::reverse_iterator(IteratorImpl<const ValueType>());
    }
    return std::reverse_iterator(IteratorImpl<const ValueType>(this->array_ + this->logical_size_));
  }

  constexpr ReverseIterator REnd() noexcept {
    if (Empty()) {
      return std::reverse_iterator(IteratorImpl<ValueType>());
    }
    return std::reverse_iterator(IteratorImpl<ValueType>(this->array_));
  }

  constexpr ConstReverseIterator REnd() const noexcept {
    if (Empty()) {
      return std::reverse_iterator(IteratorImpl<const ValueType>());
    }
    return std::reverse_iterator(IteratorImpl<const ValueType>(this->array_));
  }

  [[nodiscard("Reason: Return value indicates if vector is empty")]] constexpr bool Empty(
  ) const noexcept {
    return this->logical_size_ == 0;
  }

  [[nodiscard("Reason: Return value shows current size of vector")]] constexpr SizeType Size(
  ) const noexcept {
    return this->logical_size_;
  }

  constexpr void Reserve(SizeType new_cap) {
    if (new_cap <= this->Capacity()) {
      return;
    }
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    auto extended_block = new ValueType[this->capacity_];
    for (SizeType i = 0; i < this->Size(); i++) {
      extended_block[i] = this->At(i);
    }
    delete[] this->array_;
    this->capacity_ = new_cap;
    this->array_ = extended_block;
  }

  [[nodiscard("Reason: Return value shows current capacity of vector")]] SizeType Capacity() const {
    return this->capacity_;
  }

  constexpr void Clear() noexcept {
    this->logical_size_ = 0;
  }

  // constexpr Iterator Insert(ConstIterator pos, ConstReference value) {

  // }

  constexpr void PushBack(ConstReference value) {
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

  constexpr void PopBack() {
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