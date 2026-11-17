#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include "monoid/detail/checked_arithmetic.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_d
// Symbols are values in [0, AlphabetSize). Concatenation costs O(AlphabetSize).
template <std::size_t AlphabetSize>
struct FiniteAlphabetInversions {
	static_assert(AlphabetSize > 0);
	struct value_type {
		std::array<long long, AlphabetSize> frequencies{};
		long long inversions = 0;
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_leaf(std::size_t symbol) {
		assert(symbol < AlphabetSize);
		value_type result;
		result.frequencies[symbol] = 1;
		return result;
	}
	static value_type combine(const value_type &left, const value_type &right) {
		using namespace monoid_detail;
		value_type result;
		result.inversions = checked_add(left.inversions, right.inversions);
		long long smaller_on_right = 0;
		for (std::size_t symbol = 0; symbol < AlphabetSize; ++symbol) {
			result.frequencies[symbol] = checked_add(left.frequencies[symbol], right.frequencies[symbol]);
			result.inversions = checked_multiply_add(left.frequencies[symbol], smaller_on_right, result.inversions);
			smaller_on_right = checked_add(smaller_on_right, right.frequencies[symbol]);
		}
		return result;
	}
};
