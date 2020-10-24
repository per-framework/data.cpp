#include "data_v1/array.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace data_v1;

template <class T> void test_array() {
  T an_array[] = {3, 1, 4};

  verify(size(an_array) == 3);

  verify(*begin(an_array) == 3);
  verify(end(an_array)[-1] == 4);

  verify(front(an_array) == 3);
  verify(back(an_array) == 4);
}

auto test_mutable_array = test(test_array<int>);
auto test_const_array = test(test_array<const int>);
