#pragma once

#include "data_v1/strided_iterator.hpp"

template <class Value, ptrdiff_t Stride, size_t Extent>
data_v1::strided_array<Value, Stride, Extent>::strided_array(Value *begin,
                                                             ptrdiff_t step,
                                                             size_t size) {
  this->m_pointer = begin;

  (void)step;
  if constexpr (Stride == dynamic_stride)
    this->m_step = step;
  else
    assert(Stride == step);

  (void)size;
  if constexpr (Extent == dynamic_extent)
    this->m_size = size;
  else
    assert(Extent == size);
}

template <class Value, ptrdiff_t Stride, size_t Extent>
template <class ThatValue>
data_v1::strided_array<Value, Stride, Extent>::strided_array(ThatValue *begin,
                                                             ThatValue *end)
    : strided_array(begin, sizeof(ThatValue), end - begin) {}

template <class Value, ptrdiff_t Stride, size_t Extent>
template <class ThatValue, size_t ThatExtent>
data_v1::strided_array<Value, Stride, Extent>::strided_array(
    ThatValue (&that)[ThatExtent]) {
  this->m_pointer = that;

  if constexpr (Stride == dynamic_stride)
    this->m_step = sizeof(ThatValue);
  else
    static_assert(Stride == sizeof(ThatValue));

  if constexpr (Extent == dynamic_extent)
    this->m_size = ThatExtent;
  else
    static_assert(Extent == ThatExtent);
}

template <class Value, ptrdiff_t Stride, size_t Extent>
template <class ThatValue, ptrdiff_t ThatStride, size_t ThatExtent>
data_v1::strided_array<Value, Stride, Extent>::strided_array(
    const strided_array<ThatValue, ThatStride, ThatExtent> &that) {
  this->m_pointer = that.m_pointer;

  if constexpr (Stride == dynamic_stride)
    this->m_step = that.step();
  else
    static_assert(Stride == ThatStride);

  if constexpr (Extent == dynamic_extent)
    this->m_size = that.size();
  else
    static_assert(Extent == ThatExtent);
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::empty() const {
  return !size();
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::size() const {
  if constexpr (Extent == dynamic_extent)
    return this->m_size;
  else
    return Extent;
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::step() const {
  if constexpr (Stride == dynamic_stride)
    return this->m_step;
  else
    return Stride;
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto &
data_v1::strided_array<Value, Stride, Extent>::operator[](size_t i) const {
  assert(i < size());
  return *(Value *)((char *)(this->m_pointer) + i * step());
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::begin() const {
  return strided_iterator<Value, Stride>((Value *)(this->m_pointer), step());
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::end() const {
  return strided_iterator<Value, Stride>(
      (Value *)((char *)(this->m_pointer) + size() * step()), step());
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::rbegin() const {
  auto pointer =
      (Value *)((char *)(this->m_pointer) - step() + size() * step());
  if constexpr (Stride == dynamic_stride)
    return strided_iterator<Value, Stride>(pointer, -step());
  else
    return strided_iterator<Value, -Stride>(pointer, -step());
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::strided_array<Value, Stride, Extent>::rend() const {
  // WARNING: The following computes a pointer that potentially points outside
  // of the underlying array.
  auto pointer = (Value *)((char *)(this->m_pointer) - step());
  if constexpr (Stride == dynamic_stride)
    return strided_iterator<Value, Stride>(pointer, -step());
  else
    return strided_iterator<Value, -Stride>(pointer, -step());
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto &data_v1::strided_array<Value, Stride, Extent>::front() const {
  assert(!empty());
  return operator[](0);
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto &data_v1::strided_array<Value, Stride, Extent>::back() const {
  assert(!empty());
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
          size_t Extent,
          class Struct,
          class Member>
auto data_v1::focused_on(Member(Struct::*member),
                         const strided_array<Value, Stride, Extent> &array) {
  return strided_array<Member, Stride, Extent>(
      &(array[0].*member), array.step(), array.size());
}

template <class Value, ptrdiff_t Stride, size_t Extent>
auto data_v1::reversed(const strided_array<Value, Stride, Extent> &array) {
  if constexpr (Stride == dynamic_stride)
    return strided_array<Value, Stride, Extent>(
        &array[array.size() - 1], -array.step(), array.size());
  else
    return strided_array<Value, -Stride, Extent>(
        &array[array.size() - 1], -array.step(), array.size());
}
