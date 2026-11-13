#pragma once
#include "acted_monoid/detail/product_numeric_update.hpp"

// Read the product from value. Mapping a nonempty range takes O(log count).
template <typename T = long long>
using ProductMultiply = acted_monoid_detail::ProductNumericUpdate<T, false>;
