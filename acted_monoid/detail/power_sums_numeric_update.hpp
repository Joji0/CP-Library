#pragma once
#include <array>
#include <cstddef>
#include <limits>
#include <type_traits>
#include "acted_monoid/detail/numeric_action.hpp"
#include "acted_monoid/power_sums_assign.hpp"
#include "monoid/detail/checked_power.hpp"

namespace acted_monoid_detail {
template <typename T, std::size_t Degree, NumericUpdate Update>
struct PowerSumsNumericUpdate : PowerSumsAssign<T, Degree>, NumericAction<T, Update> {
	static_assert(Update == NumericUpdate::add || Update == NumericUpdate::multiply || Update == NumericUpdate::affine);
	using Base = PowerSumsAssign<T, Degree>;
	using Actions = NumericAction<T, Update>;
	using Value = typename Base::Value;
	using Action = typename Actions::Action;
	using Actions::composition;
	using Actions::identity_action;

	static Value mapping(const Action &action, Value value) {
		if (value.count == 0) return value;
		const auto [factor, offset] = Actions::coefficients(action);
		if (factor == T(1) && offset == T(0)) return value;
		if (factor == T(0)) return Base::mapping(offset, value);
		if (offset == T(0)) {
			for (std::size_t degree = 1; degree <= Degree; ++degree) {
				value.powers[degree] = monoid_detail::checked_multiply_by_power(value.powers[degree], factor, degree);
			}
			return value;
		}

		using WideInteger = std::conditional_t<std::numeric_limits<T>::is_signed, __int128, unsigned __int128>;
		using Work = std::conditional_t<monoid_detail::builtin_integer<T> && sizeof(T) <= sizeof(long long), WideInteger, T>;
		const Work slope(factor), shift(offset);
		const Work count = monoid_detail::checked_count<Work>(value.count);
		// Coefficients of (slope * x + shift)^degree. Build successive rows in place.
		std::array<Work, Degree + 1> coefficient{};
		coefficient[0] = Work(1);
		Value result = value;
		for (std::size_t degree = 1; degree <= Degree; ++degree) {
			for (std::size_t term = degree; term > 0; --term) {
				coefficient[term] = monoid_detail::checked_multiply_add(slope, coefficient[term - 1],
											monoid_detail::checked_multiply(shift, coefficient[term]));
			}
			coefficient[0] = monoid_detail::checked_multiply(shift, coefficient[0]);
			Work sum = monoid_detail::checked_multiply(coefficient[0], count);
			for (std::size_t term = 1; term <= degree; ++term) {
				sum = monoid_detail::checked_multiply_add(coefficient[term], Work(value.powers[term]), sum);
			}
			result.powers[degree] = monoid_detail::checked_count<T>(sum);
		}
		return result;
	}
};
}  // namespace acted_monoid_detail
