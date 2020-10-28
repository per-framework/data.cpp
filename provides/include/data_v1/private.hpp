#pragma once

#include <cstddef>
#include <limits>
#include <type_traits>
#include <variant>

namespace data_v1 {

inline constexpr ptrdiff_t dynamic_stride =
    std::numeric_limits<ptrdiff_t>::min();

inline constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

template <class Thunk> struct lazy_t;
template <class Value, ptrdiff_t Stride> struct strided_iterator_t;
template <class Value, ptrdiff_t Stride, size_t Size> struct strided_t;

class Private {
  template <class> friend struct data_v1::lazy_t;

  template <class Thunk> class lazy {
    template <class> friend struct data_v1::lazy_t;

    template <class ThatThunk>
    lazy(ThatThunk &&thunk) : m_state(std::forward<ThatThunk>(thunk)) {}

    std::variant<Thunk, std::invoke_result_t<Thunk>> m_state;
  };

  //

  template <class, ptrdiff_t> friend struct data_v1::strided_iterator_t;
  template <class, ptrdiff_t, size_t> friend struct data_v1::strided_t;

  template <class Value> class pointer_member {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator_t;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_t;

    Value *m_pointer;
  };

  template <ptrdiff_t Stride, class Base> class step_member : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator_t;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_t;
  };

  template <class Base> class step_member<dynamic_stride, Base> : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator_t;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_t;

    ptrdiff_t m_step;
  };

  template <size_t Size, class Base> class size_member : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator_t;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_t;
  };

  template <class Base> class size_member<dynamic_extent, Base> : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator_t;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_t;

    size_t m_size;
  };

  template <class Value, ptrdiff_t Stride>
  using strided_iterator = step_member<Stride, pointer_member<Value>>;

  template <class Value, ptrdiff_t Stride, size_t Size>
  using strided = size_member<Size, step_member<Stride, pointer_member<Value>>>;
};

} // namespace data_v1
