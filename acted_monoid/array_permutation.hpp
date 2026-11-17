#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <utility>
#include "monoid/array_monoid.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_l
// Permutes 1-based components of any ArrayMonoid. action[destination] is the source.
template <typename Monoid, std::size_t Size>
struct ArrayPermutation {
	static_assert(Size > 0);
	using Aggregate = ArrayMonoid<Monoid, Size>;
	using Value = typename Aggregate::value_type;
	using Action = std::array<std::size_t, Size + 1>;

	static Value identity() { return Aggregate::identity(); }
	static Value combine(const Value &left, const Value &right) { return Aggregate::combine(left, right); }

	static Action identity_action() {
		Action result{};
		for (std::size_t i = 1; i <= Size; ++i) result[i] = i;
		return result;
	}
	static Action make_swap(std::size_t first, std::size_t second) {
		assert(1 <= first && first <= Size && 1 <= second && second <= Size);
		Action result = identity_action();
		std::swap(result[first], result[second]);
		return result;
	}
	static Value mapping(const Action &action, const Value &value) {
		Value result = identity();
		for (std::size_t i = 1; i <= Size; ++i) result[i] = value[action[i]];
		return result;
	}
	static Action composition(const Action &later, const Action &earlier) {
		Action result{};
		for (std::size_t i = 1; i <= Size; ++i) result[i] = earlier[later[i]];
		return result;
	}
};
