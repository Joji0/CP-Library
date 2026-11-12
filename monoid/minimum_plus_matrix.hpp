#pragma once
#include "monoid/detail/tropical_matrix.hpp"

template <typename T, std::size_t Size>
using MinimumPlusMatrix = monoid_detail::TropicalMatrix<T, Size, false>;
