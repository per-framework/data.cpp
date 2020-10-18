#pragma once

#include "synopsis.hpp"

template <class Struct, class Value>
auto *data_v1::pointer_of(Value(Struct::*member), Struct *whole) {
  return &(whole->*member);
}
