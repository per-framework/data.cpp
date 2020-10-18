#pragma once

#include "data_v1/private.hpp"

namespace data_v1 {

// array.hpp ===================================================================

template <class Value, size_t N> constexpr auto size(Value (&array)[N]);

template <class Value, size_t N> auto begin(Value (&array)[N]);
template <class Value, size_t N> auto end(Value (&array)[N]);

// strided_iterator.hpp ========================================================

template <class Value>
struct strided_iterator : Private::strided_iterator<Value> {
  using value_type = Value;

  strided_iterator(Value *begin, ptrdiff_t stride);

  auto &operator*() const;

  static auto equals(const strided_iterator &lhs, const strided_iterator &rhs);

  auto increment();
  auto decrement();
};

template <class Value>
auto operator==(const strided_iterator<Value> &lhs,
                const strided_iterator<Value> &rhs);

template <class Value>
auto operator!=(const strided_iterator<Value> &lhs,
                const strided_iterator<Value> &rhs);

template <class Value> auto &operator++(strided_iterator<Value> &i);
template <class Value> auto &&operator++(strided_iterator<Value> &&i);

template <class Value> auto operator++(strided_iterator<Value> &i, int);

template <class Value> auto &operator--(strided_iterator<Value> &i);
template <class Value> auto &&operator--(strided_iterator<Value> &&i);

template <class Value> auto operator--(strided_iterator<Value> &i, int);

// strided_array.hpp ===========================================================

template <class Value> struct strided_array : Private::strided_array<Value> {
  using value_type = Value;
  using iterator_type = strided_iterator<Value>;

  strided_array(Value *begin, ptrdiff_t stride, size_t count);

  template <size_t N> explicit strided_array(Value (&array)[N]);

  strided_array(Value *begin, Value *end);

  auto size() const;
  auto stride() const;

  auto &operator[](size_t i) const;

  auto begin() const;
  auto end() const;

  auto rbegin() const;
  auto rend() const;

  auto &front() const;
  auto &back() const;
};

template <class Value, size_t N> auto make_strided_array(Value (&array)[N]);

template <class Value> auto make_strided_array(Value *begin, Value *end);

template <class Value, class Member>
auto focus_on(Member(Value::*member), const strided_array<Value> &array);

template <class Value> auto invert(const strided_array<Value> &array);

// struct.hpp ==================================================================

template <class Struct, class Value>
auto *pointer_of(Value(Struct::*member), Struct *whole = nullptr);

} // namespace data_v1
