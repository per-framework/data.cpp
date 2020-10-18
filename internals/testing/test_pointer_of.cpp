#include "data_v1/struct.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace data_v1;

auto test_pointer_of = test([]() {
  struct foo {
    int bar;
    float baz;
  } bar;

  verify(pointer_of(&foo::bar, &bar) == &bar.bar);
});
