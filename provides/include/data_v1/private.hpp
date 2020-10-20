#pragma once

#include <limits>

#include <cstddef>

namespace data_v1 {

inline constexpr ptrdiff_t dynamic_stride =
    std::numeric_limits<ptrdiff_t>::min();

inline constexpr size_t dynamic_extent = std::numeric_limits<size_t>::max();

template <class Value, ptrdiff_t Stride> struct strided_iterator;
template <class Value, ptrdiff_t Stride, size_t Size> struct strided_array;

class Private {
  template <class, ptrdiff_t> friend struct strided_iterator;
  template <class, ptrdiff_t, size_t> friend struct strided_array;

  template <class Value> class pointer_member {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_array;

    Value *m_pointer;
  };

  template <ptrdiff_t Stride, class Base> class stride_member : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_array;
  };

  template <class Base> class stride_member<dynamic_stride, Base> : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_array;

    ptrdiff_t m_stride;
  };

  template <size_t Size, class Base> class size_member : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_array;
  };

  template <class Base> class size_member<dynamic_extent, Base> : Base {
    template <class, ptrdiff_t> friend struct data_v1::strided_iterator;
    template <class, ptrdiff_t, size_t> friend struct data_v1::strided_array;

    size_t m_size;
  };

  template <class Value, ptrdiff_t Stride>
  using strided_iterator = stride_member<Stride, pointer_member<Value>>;

  template <class Value, ptrdiff_t Stride, size_t Size>
  using strided_array =
      size_member<Size, stride_member<Stride, pointer_member<Value>>>;
};

} // namespace data_v1
