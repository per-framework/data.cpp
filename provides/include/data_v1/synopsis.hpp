#pragma once

#include <limits>

#include "data_v1/private.hpp"

namespace data_v1 {

// array.hpp ===================================================================

template <class Value, size_t N> constexpr auto size(Value (&array)[N]);

template <class Value, size_t N> auto begin(Value (&array)[N]);
template <class Value, size_t N> auto end(Value (&array)[N]);

// strided_iterator.hpp ========================================================

template <class Value, ptrdiff_t Stride = dynamic_stride>
struct strided_iterator : Private::strided_iterator<Value, Stride> {
  using value_type = Value;

  strided_iterator(Value *begin, ptrdiff_t stride);

  auto stride() const;

  auto &operator*() const;

  static auto equals(const strided_iterator &lhs, const strided_iterator &rhs);

  auto increment();
  auto decrement();
};

template <class Value, ptrdiff_t Stride>
auto operator==(const strided_iterator<Value, Stride> &lhs,
                const strided_iterator<Value, Stride> &rhs);

template <class Value, ptrdiff_t Stride>
auto operator!=(const strided_iterator<Value, Stride> &lhs,
                const strided_iterator<Value, Stride> &rhs);

template <class Value, ptrdiff_t Stride>
auto &operator++(strided_iterator<Value, Stride> &i);

template <class Value, ptrdiff_t Stride>
auto &&operator++(strided_iterator<Value, Stride> &&i);

template <class Value, ptrdiff_t Stride>
auto operator++(strided_iterator<Value, Stride> &i, int);

template <class Value, ptrdiff_t Stride>
auto &operator--(strided_iterator<Value, Stride> &i);

template <class Value, ptrdiff_t Stride>
auto &&operator--(strided_iterator<Value, Stride> &&i);

template <class Value, ptrdiff_t Stride>
auto operator--(strided_iterator<Value, Stride> &i, int);

// strided_array.hpp ===========================================================

template <class Value,
          ptrdiff_t Stride = dynamic_stride,
          size_t Size = dynamic_extent>
struct strided_array : Private::strided_array<Value, Stride, Size> {
  using element_type = Value;
  using iterator_type = strided_iterator<Value>;

  strided_array(Value *begin, ptrdiff_t stride, size_t count);

  template <class ThatValue> strided_array(ThatValue *begin, ThatValue *end);

  template <class ThatValue, size_t ThatSize>
  strided_array(ThatValue (&that)[ThatSize]);

  template <class ThatValue, ptrdiff_t ThatStride, size_t ThatSize>
  strided_array(const strided_array<ThatValue, ThatStride, ThatSize> &that);

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

template <class Value, size_t Size = dynamic_extent>
using contiguous_array = strided_array<Value, sizeof(Value), Size>;

template <class Value, size_t N> auto make_strided_array(Value (&array)[N]);

template <class Value> auto make_strided_array(Value *begin, Value *end);

template <class Value,
          ptrdiff_t Stride,
          size_t Size,
          class Struct,
          class Member>
auto focus_on(Member(Struct::*member),
              const strided_array<Value, Stride, Size> &array);

template <class Value, ptrdiff_t Stride, size_t Size>
auto reversed(const strided_array<Value, Stride, Size> &array);

// struct.hpp ==================================================================

template <class Struct, class Value>
auto *pointer_of(Value(Struct::*member), Struct *whole = nullptr);

} // namespace data_v1
