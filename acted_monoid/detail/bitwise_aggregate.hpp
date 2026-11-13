#pragma once
#include <cstddef>
#include "acted_monoid/detail/bitwise_action.hpp"

namespace acted_monoid_detail {
enum class BitwiseQuery { bitwise_and,
			  bitwise_or,
			  exclusive_or };

// The three masks and segment parity determine all bitwise queries after any truth table.
template <typename T, std::size_t Bits, BitwiseUpdate Update, BitwiseQuery Query>
struct BitwiseAggregate : BitwiseAction<T, Bits, Update> {
	using Base = BitwiseAction<T, Bits, Update>;
	using Domain = typename Base::Domain;
	using Unsigned = typename Base::Unsigned;
	using Action = typename Base::Action;
	static constexpr Unsigned mask = Domain::mask;
	struct Value {
		Unsigned bitwise_and = mask;
		Unsigned bitwise_or = 0;
		Unsigned exclusive_or = 0;
		int count = 0;
		bool operator==(const Value &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(T value = T(0)) {
		const Unsigned bits = Domain::encode(value);
		return {bits, bits, bits, 1};
	}
	static Value combine(const Value &left, const Value &right) {
		return {Unsigned(left.bitwise_and & right.bitwise_and), Unsigned(left.bitwise_or | right.bitwise_or),
			Unsigned(left.exclusive_or ^ right.exclusive_or), monoid_detail::checked_add(left.count, right.count)};
	}
	static Value mapping(Action action, Value value) {
		if (value.count == 0) return value;
		const auto [from_zero, from_one] = Base::truth_table(action);
		const Unsigned bitwise_and = (~value.bitwise_or | from_one) & (value.bitwise_and | from_zero) & mask;
		const Unsigned bitwise_or = (value.bitwise_or & from_one) | (~value.bitwise_and & from_zero);
		const Unsigned exclusive_or = (value.exclusive_or & (from_zero ^ from_one)) ^ (value.count % 2 ? from_zero : 0);
		return {bitwise_and, bitwise_or, exclusive_or, value.count};
	}
	static T product(const Value &value) {
		if constexpr (Query == BitwiseQuery::bitwise_and) return static_cast<T>(value.bitwise_and);
		if constexpr (Query == BitwiseQuery::bitwise_or) return static_cast<T>(value.bitwise_or);
		if constexpr (Query == BitwiseQuery::exclusive_or) return static_cast<T>(value.exclusive_or);
	}
};
}  // namespace acted_monoid_detail
