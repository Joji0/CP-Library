#pragma once
#include <array>
#include <cstddef>
#include <type_traits>
#include "acted_monoid/detail/bitwise_action.hpp"

namespace acted_monoid_detail {
template <typename T>
using DefaultBitwiseSum = std::conditional_t<std::is_signed_v<T>, long long, unsigned long long>;

// Each node stores the number of ones in each bit. Combine and map take O(Bits).
template <typename T, std::size_t Bits, typename Sum, BitwiseUpdate Update>
struct BitwiseSum : BitwiseAction<T, Bits, Update> {
	using Base = BitwiseAction<T, Bits, Update>;
	using Domain = typename Base::Domain;
	using Unsigned = typename Base::Unsigned;
	using Action = typename Base::Action;
	static_assert(monoid_detail::builtin_integer<Sum> && !std::is_same_v<Sum, bool>);

	struct Value {
		std::array<int, Bits> ones{};
		int count = 0;
		bool operator==(const Value &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(T value = T(0)) {
		const Unsigned bits = Domain::encode(value);
		Value leaf;
		leaf.count = 1;
		for (std::size_t bit = 0; bit < Bits; ++bit) leaf.ones[bit] = (bits >> bit) & 1;
		return leaf;
	}
	static Value combine(const Value &left, const Value &right) {
		Value result;
		result.count = monoid_detail::checked_add(left.count, right.count);
		for (std::size_t bit = 0; bit < Bits; ++bit) result.ones[bit] = left.ones[bit] + right.ones[bit];
		return result;
	}
	static Value mapping(Action action, Value value) {
		if (value.count == 0) return value;
		const auto [from_zero, from_one] = Base::truth_table(action);
		if (from_zero == 0 && from_one == Domain::mask) return value;
		for (std::size_t bit = 0; bit < Bits; ++bit) {
			const int zeros = ((from_zero >> bit) & 1) ? value.count - value.ones[bit] : 0;
			const int ones = ((from_one >> bit) & 1) ? value.ones[bit] : 0;
			value.ones[bit] = zeros + ones;
		}
		return value;
	}
	// At most INT_MAX values of at most 64 bits fit in the wide accumulator.
	static __int128 wide_sum(const Value &value) {
		__int128 result = 0;
		for (std::size_t bit = 0; bit < Bits; ++bit) {
			const __int128 contribution = (__int128(1) << bit) * value.ones[bit];
			const bool sign_bit = std::is_signed_v<T> && Bits == bit_width<T> && bit + 1 == Bits;
			result += sign_bit ? -contribution : contribution;
		}
		return result;
	}
	static Sum sum(const Value &value) { return monoid_detail::checked_count<Sum>(wide_sum(value)); }
};
}  // namespace acted_monoid_detail
