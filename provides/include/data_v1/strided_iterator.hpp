#pragma once

#include "data_v1/synopsis.hpp"

#include <cassert>

template <class Value, ptrdiff_t Stride>
data_v1::strided_iterator<Value, Stride>::strided_iterator(Value *begin,
                                                           ptrdiff_t stride) {
  this->m_pointer = (void *)begin;

  (void)stride;
  if constexpr (Stride == dynamic_stride)
    this->m_stride = stride;
  else
    assert(Stride == stride);
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator<Value, Stride>::stride() const {
  if constexpr (Stride == dynamic_stride)
    return this->m_stride;
  else
    return Stride;
}

template <class Value, ptrdiff_t Stride>
auto &data_v1::strided_iterator<Value, Stride>::operator*() const {
  return *(Value *)(this->m_pointer);
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator<Value, Stride>::equals(
    const strided_iterator<Value, Stride> &lhs,
    const strided_iterator<Value, Stride> &rhs) {
  if constexpr (Stride != dynamic_stride)
    assert(lhs.m_stride == rhs.m_stride);
  return lhs.m_pointer == rhs.m_pointer;
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator<Value, Stride>::increment() {
  this->m_pointer = (void *)((char *)(this->m_pointer) + stride());
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator<Value, Stride>::decrement() {
  this->m_pointer = (void *)((char *)(this->m_pointer) - stride());
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator==(const strided_iterator<Value, Stride> &lhs,
                         const strided_iterator<Value, Stride> &rhs) {
  return strided_iterator<Value, Stride>::equals(lhs, rhs);
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator!=(const strided_iterator<Value, Stride> &lhs,
                         const strided_iterator<Value, Stride> &rhs) {
  return !strided_iterator<Value, Stride>::equals(lhs, rhs);
}

template <class Value, ptrdiff_t Stride>
auto &data_v1::operator++(strided_iterator<Value, Stride> &it) {
  it.increment();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto &&data_v1::operator++(strided_iterator<Value, Stride> &&it) {
  it.increment();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator++(strided_iterator<Value, Stride> &it, int) {
  auto result = it;
  it.increment();
  return result;
}

template <class Value, ptrdiff_t Stride>
auto &data_v1::operator--(strided_iterator<Value, Stride> &it) {
  it.decrement();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto &&data_v1::operator--(strided_iterator<Value, Stride> &&it) {
  it.decrement();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator--(strided_iterator<Value, Stride> &it, int) {
  auto result = it;
  it.decrement();
  return result;
}
