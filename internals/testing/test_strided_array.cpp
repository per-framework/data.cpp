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

  auto bazes = reversed(focus_on(&foo::baz, make_strided_array(foos)));

  static_assert(sizeof(bazes) == sizeof(void *));

  verify(bazes.size() == 3);
  verify(bazes.stride() == -static_cast<ptrdiff_t>(sizeof(foo)));

  verify(bazes[0] == 6.0f);
  verify(bazes[1] == 4.0f);
  verify(bazes[2] == 2.0f);

  verify(*bazes.begin() == 6.0f);
  verify(*--bazes.rend() == 6.0f);
  verify(*bazes.rbegin() == 2.0f);
  verify(*--bazes.end() == 2.0f);

  strided_array<float> dynamic = bazes;

  static_assert(sizeof(reversed(dynamic)) ==
                sizeof(void *) + sizeof(ptrdiff_t) + sizeof(size_t));

  verify(dynamic.size() == 3);
  verify(dynamic.stride() == -static_cast<ptrdiff_t>(sizeof(foo)));

  verify(dynamic[0] == 6.0f);
  verify(dynamic[1] == 4.0f);
  verify(dynamic[2] == 2.0f);

  verify(*dynamic.begin() == 6.0f);
  verify(*--dynamic.rend() == 6.0f);
  verify(*dynamic.rbegin() == 2.0f);
  verify(*--dynamic.end() == 2.0f);
});
