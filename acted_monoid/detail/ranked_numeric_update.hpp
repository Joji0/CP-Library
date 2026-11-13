#pragma once
#include <algorithm>
#include <optional>
#include <type_traits>
#include "acted_monoid/detail/numeric_action.hpp"
#include "monoid/detail/distinct_value_counts.hpp"
#include "monoid/detail/extreme_values.hpp"

namespace acted_monoid_detail {
template <typename T, int Capacity, bool Largest, bool Distinct, bool Counts, NumericUpdate Update>
struct RankedNumericUpdate : NumericAction<T, Update> {
	static_assert(!(Distinct || Counts) || Update == NumericUpdate::assign || monoid_detail::builtin_integer<T>,
		      "Distinct ranks require injective arithmetic. Use an integral value type.");
	using Actions = NumericAction<T, Update>;
	using Action = typename Actions::Action;

	template <bool Descending>
	using Ordered = std::conditional_t<Counts, monoid_detail::DistinctValueCounts<T, Capacity, Descending>,
					   monoid_detail::ExtremeValues<T, Capacity, Descending, Distinct>>;
	using Smallest = typename Ordered<false>::value_type;
	using LargestValues = typename Ordered<true>::value_type;

	struct Value {
		Smallest smallest;
		LargestValues largest;
		int count = 0;
		auto kth(int rank) const {
			if constexpr (Largest)
				return largest.kth(rank);
			else
				return smallest.kth(rank);
		}
		bool operator==(const Value &) const = default;
	};

	static Value identity() { return {}; }
	static Value make_leaf(const T &value = T(0)) { return {Ordered<false>::make_leaf(value), Ordered<true>::make_leaf(value), 1}; }
	static Value combine(const Value &left, const Value &right) {
		return {Ordered<false>::combine(left.smallest, right.smallest), Ordered<true>::combine(left.largest, right.largest),
			monoid_detail::checked_add(left.count, right.count)};
	}
	static Value mapping(const Action &action, const Value &value) {
		if (value.count == 0) return value;
		const auto [factor, offset] = Actions::coefficients(action);
		if (factor == T(1) && offset == T(0)) return value;
		if (factor == T(0)) return {constant<Smallest>(offset, value.count), constant<LargestValues>(offset, value.count), value.count};
		// Negative slopes reverse the ordering, so the other end supplies the ranks.
		if (factor < T(0)) {
			return {transform<Smallest>(value.largest, factor, offset),
				transform<LargestValues>(value.smallest, factor, offset), value.count};
		}
		return {transform<Smallest>(value.smallest, factor, offset), transform<LargestValues>(value.largest, factor, offset), value.count};
	}

       private:
	template <typename Result>
	static Result constant(const T &value, int count) {
		Result result;
		if constexpr (Counts) {
			result.size = 1;
			result.entries[1] = {value, count};
		} else {
			result.count = Distinct ? 1 : std::min(count, Capacity);
			for (int i = 0; i < result.count; ++i) result.values[i + 1] = value;
		}
		return result;
	}

	template <typename Result, typename Source>
	static Result transform(const Source &source, const T &factor, const T &offset) {
		Result result;
		if constexpr (Counts) {
			result.size = std::min(source.size, Capacity);
			for (int i = 0; i < result.size; ++i) {
				const auto &entry = source.entries[i + 1];
				result.entries[i + 1] = {monoid_detail::checked_multiply_add(factor, entry.value, offset), entry.count};
			}
		} else {
			result.count = std::min(source.count, Capacity);
			for (int i = 0; i < result.count; ++i) {
				result.values[i + 1] = monoid_detail::checked_multiply_add(factor, source.values[i + 1], offset);
			}
		}
		return result;
	}
};
}  // namespace acted_monoid_detail
