#include "data_v1/lazy.hpp"

#include "testing_v1/test.hpp"

using namespace testing_v1;
using namespace data_v1;

auto test_lazy = test([]() {
  auto from_lambda = lazy([]() { return 101; });
  verify(from_lambda == 101);

  struct local {
    static int function() { return 42; }
  };

  const auto from_function = lazy(local::function);
  verify(from_function == 42);

  auto from_function_pointer = lazy(&local::function);
  verify(from_function_pointer == 42);
});
