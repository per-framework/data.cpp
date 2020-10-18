#pragma once

#include <cstddef>

namespace data_v1 {

template <class Value> struct strided_iterator;
template <class Value> struct strided_array;

class Private {
  template <class> friend struct strided_iterator;
  template <class> friend struct strided_array;

  template <class Value> class strided_iterator {
    template <class> friend struct data_v1::strided_iterator;
    template <class> friend struct data_v1::strided_array;

    void *m_pointer;
    ptrdiff_t m_stride;
  };

  template <class Value> class strided_array {
    template <class> friend struct data_v1::strided_iterator;
    template <class> friend struct data_v1::strided_array;

    void *m_pointer;
    ptrdiff_t m_stride;
    size_t m_count;
  };
};

} // namespace data_v1
