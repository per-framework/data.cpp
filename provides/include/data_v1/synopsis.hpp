#pragma once

#include "data_v1/private.hpp"

#include <array>

namespace data_v1 {

// array.hpp ===================================================================

/// Returns the number of elements in the given array.
template <class Value, size_t N> constexpr auto size(const Value (&array)[N]);

/// Returns a pointer (or iterator) to the beginning of the array.
template <class Value, size_t N> auto begin(Value (&array)[N]);

/// Returns a pointer (or iterator) to the beginning of the array.
template <class Value, size_t N> auto begin(const Value (&array)[N]);

/// Returns a pointer (or iterator) to the end of the array.
template <class Value, size_t N> auto end(Value (&array)[N]);

/// Returns a pointer (or iterator) to the end of the array.
template <class Value, size_t N> auto end(const Value (&array)[N]);

/// Returns a reference to the first element of the array.
template <class Value, size_t N> auto &front(Value (&array)[N]);

/// Returns a reference to the first element of the array.
template <class Value, size_t N> const auto &front(const Value (&array)[N]);

/// Returns a reference to the last element of the array.
template <class Value, size_t N> auto &back(Value (&array)[N]);

/// Returns a reference to the last element of the array.
template <class Value, size_t N> const auto &back(const Value (&array)[N]);

// lazy.hpp ====================================================================

/// `lazy_t<Thunk>` represents a lazily computed result.  WARNING: This is
/// currently not thread safe.
template <class Thunk> struct lazy_t : Private::lazy<Thunk> {
  using result_type = std::invoke_result_t<Thunk>;

  /// Constructs a lazily computed result.
  template <class ThatThunk> lazy_t(ThatThunk &&thunk);

  /// Computes and stores the result if that hasn't yet been done and returns a
  /// reference to it.
  operator result_type &();

  /// Computes and stores the result if that hasn't yet been done and returns a
  /// const reference to it.
  operator const result_type &() const;
};

/// Constructs a lazily computed result.
template <class Thunk> auto lazy(Thunk &&thunk);

// strided_iterator.hpp ========================================================

/// `strided_iterator_t<Value, Stride> is the iterator type of `strided_t<Value,
/// Stride, *>`.  The stride in bytes between elements can be given either
/// statically or dynamically.
template <class Value, ptrdiff_t Stride = dynamic_stride>
struct strided_iterator_t : Private::strided_iterator<Value, Stride> {
  using value_type = Value;

  /// The static value of stride in bytes or `dynamic_stride` in which case
  /// the step is only known dynamically.
  static constexpr ptrdiff_t stride = Stride;

  /// Constructs a strided iterator from a raw pointer and stride in bytes.
  strided_iterator_t(Value *pointer, ptrdiff_t step);

  /// Returns the stride in bytes between elements.
  auto step() const;

  /// Returns a reference to the current element.  The result is undefined in
  /// case the iterator does not point to an actual element in a strided array.
  auto &operator*() const;

  /// Compares `lhs` and `rhs` for equality.  Both iterators must originate from
  /// the same strided array.
  static auto equals(const strided_iterator_t &lhs,
                     const strided_iterator_t &rhs);

  /// Moves to next element.  The result is undefined in case the iterator
  /// points to beyond the last element of the underlying sequence.
  auto increment();

  /// Moves to previous element.  The result is undefined in case the iterator
  /// points to the first element of the underlying sequence.
  auto decrement();
};

/// Compares `lhs` and `rhs` for equality.  Both iterators must originate from
/// the same strided array.
template <class Value, ptrdiff_t Stride>
auto operator==(const strided_iterator_t<Value, Stride> &lhs,
                const strided_iterator_t<Value, Stride> &rhs);

/// Compares `lhs` and `rhs` for inequality.  Both iterators must originate from
/// the same strided array.
template <class Value, ptrdiff_t Stride>
auto operator!=(const strided_iterator_t<Value, Stride> &lhs,
                const strided_iterator_t<Value, Stride> &rhs);

/// Moves to next element.  The result is undefined in case the iterator
/// points to beyond the last element of the underlying sequence.
template <class Value, ptrdiff_t Stride>
auto &operator++(strided_iterator_t<Value, Stride> &i);

/// Moves to next element.  The result is undefined in case the iterator
/// points to beyond the last element of the underlying sequence.
template <class Value, ptrdiff_t Stride>
auto &&operator++(strided_iterator_t<Value, Stride> &&i);

/// Moves to next element and returns a copy of the given iterator before
/// moving.  The effect is undefined in case the iterator points to beyond the
/// last element of the underlying sequence.
template <class Value, ptrdiff_t Stride>
auto operator++(strided_iterator_t<Value, Stride> &i, int);

/// Moves to previous element.  The result is undefined in case the iterator
/// points to the first element of the underlying sequence.
template <class Value, ptrdiff_t Stride>
auto &operator--(strided_iterator_t<Value, Stride> &i);

/// Moves to previous element.  The result is undefined in case the iterator
/// points to the first element of the underlying sequence.
template <class Value, ptrdiff_t Stride>
auto &&operator--(strided_iterator_t<Value, Stride> &&i);

/// Moves to previous element and returns a copy of the given iterator before
/// moving.  The effect is undefined in case the iterator points to the first
/// element of the underlying sequence.
template <class Value, ptrdiff_t Stride>
auto operator--(strided_iterator_t<Value, Stride> &i, int);

// strided.hpp =================================================================

/// A `strided_t<Value, Stride, Extent>` is basically a generalization of both
/// `std::span<Value, Extent>` and `std::array<Value, Extent>` and is a view of
/// or a reference to a subsequence of elements of an array.  Both the stride in
/// bytes between elements and the extent or number of elements can be given
/// either statically or dynamically.  In case the stride is given statically,
/// and is `sizeof(Value)`, then the array is contiguous.
template <class Value,
          ptrdiff_t Stride = dynamic_stride,
          size_t Extent = dynamic_extent>
struct strided_t : Private::strided<Value, Stride, Extent> {
  using element_type = Value;
  using iterator_type = strided_iterator_t<Value, Stride>;

  /// The static value of stride in bytes or `dynamic_stride` in which case
  /// the step is only known dynamically.
  static constexpr ptrdiff_t stride = Stride;

  /// The static value of extent  or `dynamic_extent` in which case
  /// the size is only known dynamically.
  static constexpr size_t extent = Extent;

  /// Constructs a strided array from a raw pointer, stride in bytes, and number
  /// of elements.
  strided_t(Value *begin, ptrdiff_t step, size_t size);

  /// Constructs a strided array from two raw pointers.
  template <class ThatValue> strided_t(ThatValue *begin, ThatValue *end);

  /// Constructs a strided array from specified array.
  template <class ThatValue, size_t ThatExtent>
  strided_t(ThatValue (&that)[ThatExtent]);

  /// Constructs a strided array from given strided array (of different type).
  template <class ThatValue, ptrdiff_t ThatStride, size_t ThatExtent>
  strided_t(const strided_t<ThatValue, ThatStride, ThatExtent> &that);

  /// Constructs a strided array from a given `std::array`.
  template <class ThatValue, size_t ThatExtent>
  strided_t(std::array<ThatValue, ThatExtent> &array);

  /// Constructs a strided array from a given `std::array`.
  template <class ThatValue, size_t ThatExtent>
  strided_t(const std::array<ThatValue, ThatExtent> &array);

  /// Checks if the sequence is empty.
  auto empty() const;

  /// Returns the number of elements in the sequence.
  auto size() const;

  /// Returns the stride in bytes between elements.
  auto step() const;

  /// Returns reference to the `i`th element in the sequence.
  auto &operator[](size_t i) const;

  /// Returns an iterator to the beginning of the sequence.
  auto begin() const;

  /// Returns an iterator to the end of the sequence.
  auto end() const;

  /// Returns an iterator to the beginning of the reversed sequence.  See also:
  /// `reversed`.
  auto rbegin() const;

  /// Returns an iterator to the end of the reversed sequence.  See also:
  /// `reversed`.
  auto rend() const;

  /// Returns a reference to the first element of the sequence.  The result is
  /// undefined in case the sequence is empty.
  auto &front() const;

  /// Returns a reference to the last element of the sequence.  The result is
  /// undefined in case the sequence is empty.
  auto &back() const;
};

/// Type alias for a strided array whose elements are statically known to be
/// stored contiguously.
template <class Value, size_t Extent = dynamic_extent>
using contiguous_t = strided_t<Value, sizeof(Value), Extent>;

/// Constructs a strided array from a raw pointer, stride in bytes, and number
/// of elements.
template <class Value> auto strided(Value *begin, ptrdiff_t step, size_t size);

/// Constructs a strided array from two raw pointers.
template <class Value> auto strided(Value *begin, Value *end);

/// Constructs a strided array from specified array.
template <class Value, size_t N> auto strided(Value (&array)[N]);

/// Constructs a strided array from specified array.
template <class Value, size_t N> auto strided(const Value (&array)[N]);

/// Constructs a strided array from a `std::array`.
template <class Value, size_t Extent>
auto strided(std::array<Value, Extent> &array);

/// Constructs a strided array from a `std::array`.
template <class Value, size_t Extent>
auto strided(const std::array<Value, Extent> &array);

/// View of members of the given strided array of structures.
template <class SubtypeOfStruct,
          ptrdiff_t Stride,
          size_t Extent,
          class Struct,
          class Value>
auto focused_on(Value(Struct::*member),
                const strided_t<SubtypeOfStruct, Stride, Extent> &array);

/// Reversed view of the given strided array.
template <class Value, ptrdiff_t Stride, size_t Extent>
auto reversed(const strided_t<Value, Stride, Extent> &array);

// struct.hpp ==================================================================

/// Returns a pointer to the specified member of the pointed structure.
template <class Struct, class Value>
auto *pointer_of(Value(Struct::*member), Struct *whole = nullptr);

} // namespace data_v1
