#include "data_v1/strided_array.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace data_v1;

auto test_strided_array = test([]() {
  struct foo {
    int bar;
    float baz;
  };

  foo foos[] = {{1, 2.0f}, {3, 4.0f}, {5, 6.0f}};

  auto bazes = invert(focus_on(&foo::baz, make_strided_array(foos)));

  verify(bazes.size() == 3);

  verify(bazes[0] == 6.0f);
  verify(bazes[1] == 4.0f);
  verify(bazes[2] == 2.0f);

  verify(*bazes.begin() == 6.0f);
  verify(*bazes.rbegin() == 2.0f);
});
