#pragma once

#include "data_v1/synopsis.hpp"

template <class Value, size_t N> constexpr auto data_v1::size(Value (&)[N]) {
  return N;
}

template <class Value, size_t N> auto begin(Value (&array)[N]) { return array; }

template <class Value, size_t N> auto end(Value (&array)[N]) {
  return array + N;
}
