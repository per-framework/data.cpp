#pragma once

#include "data_v1/synopsis.hpp"

template <class Value, size_t N> constexpr auto data_v1::size(Value (&)[N]) {
  return N;
}

template <class Value, size_t N> auto data_v1::begin(Value (&array)[N]) {
  return array;
}

template <class Value, size_t N> auto data_v1::end(Value (&array)[N]) {
  return array + N;
}

template <class Value, size_t N> auto &data_v1::front(Value (&array)[N]) {
  return array[0];
}

template <class Value, size_t N> auto &data_v1::back(Value (&array)[N]) {
  return array[N - 1];
}
