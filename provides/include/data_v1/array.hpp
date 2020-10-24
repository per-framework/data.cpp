#pragma once

#include "data_v1/synopsis.hpp"

template <class Value, size_t N>
constexpr auto data_v1::size(const Value (&)[N]) {
  return N;
}

#define MAKE(...)                                                              \
  template <class Value, size_t N>                                             \
  auto data_v1::begin(__VA_ARGS__ Value(&array)[N]) {                          \
    return array;                                                              \
  }                                                                            \
                                                                               \
  template <class Value, size_t N>                                             \
  auto data_v1::end(__VA_ARGS__ Value(&array)[N]) {                            \
    return array + N;                                                          \
  }                                                                            \
                                                                               \
  template <class Value, size_t N>                                             \
  __VA_ARGS__ auto &data_v1::front(__VA_ARGS__ Value(&array)[N]) {             \
    return array[0];                                                           \
  }                                                                            \
                                                                               \
  template <class Value, size_t N>                                             \
  __VA_ARGS__ auto &data_v1::back(__VA_ARGS__ Value(&array)[N]) {              \
    return array[N - 1];                                                       \
  }

MAKE()
MAKE(const)
#undef MAKE
