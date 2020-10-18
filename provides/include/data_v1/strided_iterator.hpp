#pragma once

#include "data_v1/synopsis.hpp"

#include <cassert>

template <class Value>
data_v1::strided_iterator<Value>::strided_iterator(Value *begin,
                                                   ptrdiff_t stride) {
  this->m_pointer = begin;
  this->m_stride = stride;
}

template <class Value>
auto &data_v1::strided_iterator<Value>::operator*() const {
  return *static_cast<Value *>(this->m_pointer);
}

template <class Value>
auto data_v1::strided_iterator<Value>::equals(
    const strided_iterator<Value> &lhs, const strided_iterator<Value> &rhs) {
  assert(lhs.m_stride == rhs.m_stride);
  return lhs.m_pointer == rhs.m_pointer;
}

template <class Value> auto data_v1::strided_iterator<Value>::increment() {
  this->m_pointer = static_cast<void *>(static_cast<char *>(this->m_pointer) +
                                        this->m_stride);
}

template <class Value> auto data_v1::strided_iterator<Value>::decrement() {
  this->m_pointer = static_cast<void *>(static_cast<char *>(this->m_pointer) -
                                        this->m_stride);
}

template <class Value>
auto data_v1::operator==(const strided_iterator<Value> &lhs,
                         const strided_iterator<Value> &rhs) {
  return strided_iterator<Value>::equals(lhs, rhs);
}

template <class Value>
auto data_v1::operator!=(const strided_iterator<Value> &lhs,
                         const strided_iterator<Value> &rhs) {
  return !strided_iterator<Value>::equals(lhs, rhs);
}

template <class V> auto &data_v1::operator++(strided_iterator<V> &it) {
  it.increment();
  return it;
}

template <class V> auto &&data_v1::operator++(strided_iterator<V> &&it) {
  it.increment();
  return it;
}

template <class Value>
auto data_v1::operator++(strided_iterator<Value> &it, int) {
  auto result = it;
  it.increment();
  return result;
}

template <class V> auto &data_v1::operator--(strided_iterator<V> &it) {
  it.decrement();
  return it;
}

template <class V> auto &&data_v1::operator--(strided_iterator<V> &&it) {
  it.decrement();
  return it;
}

template <class Value>
auto data_v1::operator--(strided_iterator<Value> &it, int) {
  auto result = it;
  it.decrement();
  return result;
}
