#pragma once

#include "data_v1/synopsis.hpp"

template <class Thunk> data_v1::lazy_t<Thunk>::operator result_type &() {
  if (std::holds_alternative<Thunk>(this->m_state))
    this->m_state = std::get<Thunk>(this->m_state)();
  return std::get<result_type>(this->m_state);
}

template <class Thunk>
data_v1::lazy_t<Thunk>::operator const result_type &() const {
  return const_cast<lazy_t<Thunk> *>(this)->operator result_type &();
}

template <class Thunk>
template <class ThatThunk>
data_v1::lazy_t<Thunk>::lazy_t(ThatThunk &&thunk)
    : Private::lazy<Thunk>(std::forward<ThatThunk>(thunk)) {}

template <class Thunk> auto data_v1::lazy(Thunk &&thunk) {
  return lazy_t<std::decay_t<Thunk>>(std::forward<Thunk>(thunk));
}
