#pragma once

#include "data_v1/synopsis.hpp"

#include <cassert>

template <class Value, ptrdiff_t Stride>
data_v1::strided_iterator_t<Value, Stride>::strided_iterator_t(Value *pointer,
                                                               ptrdiff_t step) {
  this->m_pointer = pointer;

  (void)step;
  if constexpr (Stride == dynamic_stride)
    this->m_step = step;
  else
    assert(Stride == step);
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator_t<Value, Stride>::step() const {
  if constexpr (Stride == dynamic_stride)
    return this->m_step;
  else
    return Stride;
}

template <class Value, ptrdiff_t Stride>
auto &data_v1::strided_iterator_t<Value, Stride>::operator*() const {
  return *(this->m_pointer);
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator_t<Value, Stride>::equals(
    const strided_iterator_t<Value, Stride> &lhs,
    const strided_iterator_t<Value, Stride> &rhs) {
  if constexpr (Stride == dynamic_stride)
    assert(lhs.m_step == rhs.m_step);
  return lhs.m_pointer == rhs.m_pointer;
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator_t<Value, Stride>::increment() {
  this->m_pointer = (Value *)((char *)(this->m_pointer) + step());
}

template <class Value, ptrdiff_t Stride>
auto data_v1::strided_iterator_t<Value, Stride>::decrement() {
  this->m_pointer = (Value *)((char *)(this->m_pointer) - step());
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator==(const strided_iterator_t<Value, Stride> &lhs,
                         const strided_iterator_t<Value, Stride> &rhs) {
  return strided_iterator_t<Value, Stride>::equals(lhs, rhs);
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator!=(const strided_iterator_t<Value, Stride> &lhs,
                         const strided_iterator_t<Value, Stride> &rhs) {
  return !strided_iterator_t<Value, Stride>::equals(lhs, rhs);
}

template <class Value, ptrdiff_t Stride>
auto &data_v1::operator++(strided_iterator_t<Value, Stride> &it) {
  it.increment();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto &&data_v1::operator++(strided_iterator_t<Value, Stride> &&it) {
  it.increment();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator++(strided_iterator_t<Value, Stride> &it, int) {
  auto result = it;
  it.increment();
  return result;
}

template <class Value, ptrdiff_t Stride>
auto &data_v1::operator--(strided_iterator_t<Value, Stride> &it) {
  it.decrement();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto &&data_v1::operator--(strided_iterator_t<Value, Stride> &&it) {
  it.decrement();
  return it;
}

template <class Value, ptrdiff_t Stride>
auto data_v1::operator--(strided_iterator_t<Value, Stride> &it, int) {
  auto result = it;
  it.decrement();
  return result;
}
