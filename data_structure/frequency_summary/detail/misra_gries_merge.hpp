#pragma once
#include <algorithm>
#include <array>
#include <cstddef>
#include "monoid/detail/checked_arithmetic.hpp"

namespace frequency_summary_detail {
template <std::size_t Capacity>
void offer(std::array<int, Capacity> &keys, std::array<long long, Capacity> &counts,
	   int capacity, int key, long long count) {
	if (capacity < 0 || static_cast<std::size_t>(capacity) > Capacity || count < 0) monoid_detail::arithmetic_failure();
	if (count == 0 || capacity == 0) return;
	int available = -1;
	long long removed = count;
	for (int i = 0; i < capacity; ++i) {
		if (counts[i] == 0)
			available = i;
		else if (keys[i] == key) {
			counts[i] = monoid_detail::checked_add(counts[i], count);
			return;
		}
		removed = std::min(removed, counts[i]);
	}
	if (available == -1) {
		for (int i = 0; i < capacity; ++i) {
			counts[i] = monoid_detail::checked_subtract(counts[i], removed);
			if (counts[i] == 0) available = i;
		}
		count = monoid_detail::checked_subtract(count, removed);
	}
	if (count != 0) {
		keys[available] = key;
		counts[available] = count;
	}
}
}  // namespace frequency_summary_detail
