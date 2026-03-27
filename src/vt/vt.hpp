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

  // converting constructor
  template <typename U>
  // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
  IteratorImpl(const IteratorImpl<U>& other)
    requires std::is_convertible_v<U*, ValueType*>
      : ptr_(other.operator->()) {
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
      : array_(other.Data()), capacity_(other.Capacity()), logical_size_(other.Size()) {
    other.array_ = nullptr;
    other.capacity_ = 0;
    other.logical_size_ = 0;
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
    if (this == &other) {
      return *this;
    }
    this->capacity_ = other.Capacity();
    this->logical_size_ = other.Size();
    delete[] this->array_;
    this->array_ = other.Data();

    other.logical_size_ = 0;
    other.capacity_ = 0;
    other.array_ = nullptr;
    return *this;
  }

  constexpr Reference At(SizeType pos) {
    if (pos >= this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return this->AtUnsafe(pos);
  }

  constexpr ConstReference At(SizeType pos) const {
    if (pos >= this->logical_size_) {
      throw std::out_of_range("index out of range");
    }

    return this->AtUnsafe(pos);
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

  // copy insert
  constexpr Iterator Insert(ConstIterator pos_iter, ConstReference value) {
    ValueType copy = value;
    return Insert(pos_iter, std::move(copy));
  }

  // move insert
  constexpr Iterator Insert(ConstIterator pos_iter, ValueType&& value) {
    std::ptrdiff_t raw_pos = pos_iter - Begin();
    if (raw_pos > Size() + 1 || raw_pos < 0) {
      throw std::out_of_range("insertion pos is invalid");
    }
    SizeType pos = raw_pos;
    if (Size() == Capacity()) {
      this->capacity_ = Capacity() == 0 ? 2 : Capacity() * 2;
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
      auto extended_block = new ValueType[Capacity()];
      for (SizeType i = 0; i < pos; i++) {
        extended_block[i] = this->At(i);
      }
      extended_block[pos] = std::move(value);
      for (SizeType i = this->Size(); i > pos; i--) {
        extended_block[i] = this->At(i - 1);
      }
      delete[] this->array_;
      this->array_ = extended_block;
      this->logical_size_++;
      return IteratorImpl<ValueType>(this->array_ + pos);
    }
    for (SizeType i = this->Size(); i > pos; i--) {
      this->AtUnsafe(i) = this->At(i - 1);
    }
    this->AtUnsafe(pos) = std::move(value);
    this->logical_size_++;
    return IteratorImpl<ValueType>(this->array_ + pos);
  }

  constexpr Iterator Insert(ConstIterator pos_iter, SizeType count, ConstReference value) {
    std::ptrdiff_t raw_pos = pos_iter - Begin();
    if (raw_pos > Size() + 1 || raw_pos < 0) {
      throw std::out_of_range("insertion pos is invalid");
    }
    SizeType pos = raw_pos;
    if (count < 0) {
      throw std::out_of_range("insertion count is negative");
    }
    if (Size() + count > Capacity()) {
      this->capacity_ = Capacity() + count;
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
      auto extended_block = new ValueType[Capacity()];
      for (SizeType i = 0; i < pos; i++) {
        extended_block[i] = this->At(i);
      }
      for (SizeType i = 0; i < count; i++) {
        extended_block[pos + i] = value;
      }
      for (SizeType i = this->Size(); i > pos; i--) {
        extended_block[i + count - 1] = this->At(i - 1);
      }
      delete[] this->array_;
      this->array_ = extended_block;
      this->logical_size_ += count;
      return IteratorImpl<ValueType>(this->array_ + pos);
    }
    for (SizeType i = this->Size(); i > pos; i--) {
      this->AtUnsafe(i + count - 1) = this->At(i - 1);
    }
    for (SizeType i = 0; i < count; i++) {
      this->AtUnsafe(pos + i) = value;
    }
    this->logical_size_ += count;
    return IteratorImpl<ValueType>(this->array_ + pos);
  }

  template <std::input_iterator InputIt>
  constexpr Iterator Insert(ConstIterator pos_iter, InputIt first, InputIt last) {
    std::ptrdiff_t raw_pos = pos_iter - Begin();
    if (raw_pos > Size() + 1 || raw_pos < 0) {
      throw std::out_of_range("insertion pos is invalid");
    }
    SizeType pos = raw_pos;
    std::ptrdiff_t count = last - first;
    if (count < 0) {
      throw std::out_of_range("given iterators are reversed");
    }
    if (Size() + count > Capacity()) {
      this->capacity_ = Capacity() + count;
      // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
      auto extended_block = new ValueType[Capacity()];
      for (SizeType i = 0; i < pos; i++) {
        extended_block[i] = this->At(i);
      }
      for (auto i = first; i != last; i++) {
        extended_block[pos + (i - first)] = *i;
      }
      for (SizeType i = this->Size(); i > pos; i--) {
        extended_block[i + count - 1] = this->At(i - 1);
      }
      delete[] this->array_;
      this->array_ = extended_block;
      this->logical_size_ += count;
      return IteratorImpl<ValueType>(this->array_ + pos);
    }
    for (SizeType i = this->Size(); i > pos; i--) {
      this->AtUnsafe(i + count - 1) = this->At(i - 1);
    }
    for (auto i = first; i != last; i++) {
      this->AtUnsafe(pos + (i - first)) = *i;
    }
    this->logical_size_ += count;
    return IteratorImpl<ValueType>(this->array_ + pos);
  }

  constexpr Iterator Insert(ConstIterator pos_iter, std::initializer_list<ValueType> ilist) {
    return Insert(pos_iter, ilist.begin(), ilist.end());
  }

  constexpr void PushBack(ConstReference value) {
    this->Insert(this->End(), value);
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

  constexpr Reference AtUnsafe(SizeType pos) {
    return *(this->array_ + pos);
  }

  constexpr ConstReference AtUnsafe(SizeType pos) const {
    return *(this->array_ + pos);
  }
};
}  // namespace vt

#endif