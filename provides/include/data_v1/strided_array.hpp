#pragma once

#include "data_v1/strided_iterator.hpp"

template <class Value>
data_v1::strided_array<Value>::strided_array(Value *begin,
                                             ptrdiff_t stride,
                                             size_t count) {
  this->m_pointer = begin;
  this->m_stride = stride;
  this->m_count = count;
}

template <class Value>
template <size_t N>
data_v1::strided_array<Value>::strided_array(Value (&array)[N])
    : strided_array(array, sizeof(Value), N) {}

template <class Value>
data_v1::strided_array<Value>::strided_array(Value *begin, Value *end)
    : strided_array(begin, sizeof(Value), end - begin) {}

template <class Value> auto data_v1::strided_array<Value>::size() const {
  return this->m_count;
}

template <class Value> auto data_v1::strided_array<Value>::stride() const {
  return this->m_stride;
}

template <class Value>
auto &data_v1::strided_array<Value>::operator[](size_t i) const {
  return *reinterpret_cast<Value *>(static_cast<char *>(this->m_pointer) +
                                    i * this->m_stride);
}

template <class Value> auto data_v1::strided_array<Value>::begin() const {
  return strided_iterator<Value>(static_cast<Value *>(this->m_pointer),
                                 this->m_stride);
}

template <class Value> auto data_v1::strided_array<Value>::end() const {
  return strided_iterator<Value>(
      reinterpret_cast<Value *>(static_cast<char *>(this->m_pointer) +
                                this->m_count * this->m_stride),
      this->m_stride);
}

template <class Value> auto data_v1::strided_array<Value>::rbegin() const {
  return strided_iterator<Value>(
      reinterpret_cast<Value *>(static_cast<char *>(this->m_pointer) -
                                this->m_stride +
                                this->m_count * this->m_stride),
      -this->m_stride);
}

template <class Value> auto data_v1::strided_array<Value>::rend() const {
  // WARNING: The following computes a pointer that potentially points outside
  // of the underlying array.
  return strided_iterator<Value>(
      reinterpret_cast<Value *>(static_cast<char *>(this->m_pointer) -
                                this->m_stride),
      -this->m_stride);
}

template <class Value> auto &data_v1::strided_array<Value>::front() const {
  return operator[](0);
}

template <class Value> auto &data_v1::strided_array<Value>::back() const {
  return operator[](size() - 1);
}

template <class Value, size_t N>
auto data_v1::make_strided_array(Value (&array)[N]) {
  return strided_array<Value>(array);
}

template <class Value>
auto data_v1::make_strided_array(Value *begin, Value *end) {
  return strided_array<Value>(begin, end);
}

template <class Value, class Member>
auto data_v1::focus_on(Member(Value::*member),
                       const strided_array<Value> &array) {
  return strided_array<Member>(
      &(array[0].*member), array.stride(), array.size());
}

template <class Value> auto data_v1::invert(const strided_array<Value> &array) {
  return strided_array<Value>(
      &array[array.size() - 1], -array.stride(), array.size());
}
