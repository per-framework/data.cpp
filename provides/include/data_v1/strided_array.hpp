#pragma once

#include "data_v1/strided_iterator.hpp"

template <class Value, ptrdiff_t Stride, size_t Size>
data_v1::strided_array<Value, Stride, Size>::strided_array(Value *begin,
                                                           ptrdiff_t stride,
                                                           size_t size) {
  this->m_pointer = begin;

  (void)stride;
  if constexpr (Stride == dynamic_stride)
    this->m_stride = stride;
  else
    assert(Stride == stride);

  (void)size;
  if constexpr (Size == dynamic_extent)
    this->m_size = size;
  else
    assert(Size == size);
}

template <class Value, ptrdiff_t Stride, size_t Size>
template <class ThatValue>
data_v1::strided_array<Value, Stride, Size>::strided_array(ThatValue *begin,
                                                           ThatValue *end)
    : strided_array(begin, sizeof(ThatValue), end - begin) {}

template <class Value, ptrdiff_t Stride, size_t Size>
template <class ThatValue, size_t ThatSize>
data_v1::strided_array<Value, Stride, Size>::strided_array(
    ThatValue (&that)[ThatSize]) {
  this->m_pointer = that;

  if constexpr (Stride == dynamic_stride)
    this->m_stride = sizeof(ThatValue);
  else
    static_assert(Stride == sizeof(ThatValue));

  if constexpr (Size == dynamic_extent)
    this->m_size = ThatSize;
  else
    static_assert(Size == ThatSize);
}

template <class Value, ptrdiff_t Stride, size_t Size>
template <class ThatValue, ptrdiff_t ThatStride, size_t ThatSize>
data_v1::strided_array<Value, Stride, Size>::strided_array(
    const strided_array<ThatValue, ThatStride, ThatSize> &that) {
  this->m_pointer = that.m_pointer;

  if constexpr (Stride == dynamic_stride)
    this->m_stride = that.stride();
  else
    static_assert(Stride == ThatStride);

  if constexpr (Size == dynamic_extent)
    this->m_size = that.size();
  else
    static_assert(Size == ThatSize);
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::strided_array<Value, Stride, Size>::size() const {
  if constexpr (Size == dynamic_extent)
    return this->m_size;
  else
    return Size;
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::strided_array<Value, Stride, Size>::stride() const {
  if constexpr (Stride == dynamic_stride)
    return this->m_stride;
  else
    return Stride;
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto &data_v1::strided_array<Value, Stride, Size>::operator[](size_t i) const {
  return *(Value *)((char *)(this->m_pointer) + i * stride());
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::strided_array<Value, Stride, Size>::begin() const {
  return strided_iterator<Value, Stride>((Value *)(this->m_pointer), stride());
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::strided_array<Value, Stride, Size>::end() const {
  return strided_iterator<Value, Stride>(
      (Value *)((char *)(this->m_pointer) + size() * stride()), stride());
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::strided_array<Value, Stride, Size>::rbegin() const {
  auto pointer =
      (Value *)((char *)(this->m_pointer) - stride() + size() * stride());
  if constexpr (Stride == dynamic_stride)
    return strided_iterator<Value, Stride>(pointer, -stride());
  else
    return strided_iterator<Value, -Stride>(pointer, -stride());
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::strided_array<Value, Stride, Size>::rend() const {
  // WARNING: The following computes a pointer that potentially points outside
  // of the underlying array.
  auto pointer = (Value *)((char *)(this->m_pointer) - stride());
  if constexpr (Stride == dynamic_stride)
    return strided_iterator<Value, Stride>(pointer, -stride());
  else
    return strided_iterator<Value, -Stride>(pointer, -stride());
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto &data_v1::strided_array<Value, Stride, Size>::front() const {
  return operator[](0);
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto &data_v1::strided_array<Value, Stride, Size>::back() const {
  return operator[](size() - 1);
}

template <class Value, size_t N>
auto data_v1::make_strided_array(Value (&array)[N]) {
  return strided_array<Value, sizeof(Value), N>(array);
}

template <class Value>
auto data_v1::make_strided_array(Value *begin, Value *end) {
  return strided_array<Value, sizeof(Value), dynamic_extent>(begin, end);
}

template <class Value,
          ptrdiff_t Stride,
          size_t Size,
          class Struct,
          class Member>
auto data_v1::focus_on(Member(Struct::*member),
                       const strided_array<Value, Stride, Size> &array) {
  return strided_array<Member, Stride, Size>(
      &(array[0].*member), array.stride(), array.size());
}

template <class Value, ptrdiff_t Stride, size_t Size>
auto data_v1::reversed(const strided_array<Value, Stride, Size> &array) {
  if constexpr (Stride == dynamic_stride)
    return strided_array<Value, Stride, Size>(
        &array[array.size() - 1], -array.stride(), array.size());
  else
    return strided_array<Value, -Stride, Size>(
        &array[array.size() - 1], -array.stride(), array.size());
}
