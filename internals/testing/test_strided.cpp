#include "data_v1/strided.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace data_v1;

auto test_strided = test([]() {
  struct foo {
    int bar;
    float baz;
  };

  struct foo_derived : foo {
    bool boz;
  };

  foo_derived foo_deriveds[] = {
      {{1, 2.0f}, false}, {{3, 4.0f}, true}, {{5, 6.0f}, false}};

  strided<foo> foos = foo_deriveds;
  verify(foos.step() == sizeof(foo_derived));

  auto bazes = reversed(focused_on(&foo::baz, make_strided(foo_deriveds)));

  static_assert(sizeof(bazes) == sizeof(void *));

  verify(bazes.size() == 3);
  verify(bazes.step() == -static_cast<ptrdiff_t>(sizeof(foo_derived)));

  verify(bazes[0] == 6.0f);
  verify(bazes[1] == 4.0f);
  verify(bazes[2] == 2.0f);

  verify(*bazes.begin() == 6.0f);
  verify(*--bazes.rend() == 6.0f);
  verify(*bazes.rbegin() == 2.0f);
  verify(*--bazes.end() == 2.0f);

  strided<float> dynamic = bazes;

  static_assert(sizeof(reversed(dynamic)) ==
                sizeof(void *) + sizeof(ptrdiff_t) + sizeof(size_t));

  verify(dynamic.size() == 3);
  verify(dynamic.step() == -static_cast<ptrdiff_t>(sizeof(foo_derived)));

  verify(dynamic[0] == 6.0f);
  verify(dynamic[1] == 4.0f);
  verify(dynamic[2] == 2.0f);

  verify(*dynamic.begin() == 6.0f);
  verify(*--dynamic.rend() == 6.0f);
  verify(*dynamic.rbegin() == 2.0f);
  verify(*--dynamic.end() == 2.0f);

  strided<const float> const_dynamic = bazes;

  static_assert(sizeof(reversed(const_dynamic)) ==
                sizeof(void *) + sizeof(ptrdiff_t) + sizeof(size_t));

  verify(const_dynamic.size() == 3);
  verify(const_dynamic.step() == -static_cast<ptrdiff_t>(sizeof(foo_derived)));

  verify(const_dynamic[0] == 6.0f);
  verify(const_dynamic[1] == 4.0f);
  verify(const_dynamic[2] == 2.0f);

  verify(*const_dynamic.begin() == 6.0f);
  verify(*--const_dynamic.rend() == 6.0f);
  verify(*const_dynamic.rbegin() == 2.0f);
  verify(*--const_dynamic.end() == 2.0f);
});
