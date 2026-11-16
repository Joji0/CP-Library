#pragma once
#include <cassert>
#include <limits>
#include <vector>

// a[i] counts permutations of size i without fixed points. No division.
template <typename Value>
std::vector<Value> derangement_numbers(int maximum) {
	assert(0 <= maximum && maximum < std::numeric_limits<int>::max());
	std::vector<Value> result(maximum + 1, Value(0));
	result[0] = Value(1);
	Value sign = 1;
	for (int i = 1; i <= maximum; ++i) {
		sign = -sign;
		result[i] = Value(i) * result[i - 1] + sign;
	}
	return result;
}
