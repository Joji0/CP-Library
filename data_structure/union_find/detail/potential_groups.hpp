#pragma once
#include "monoid/add.hpp"
#include "monoid/detail/checked_arithmetic.hpp"
#include "monoid/exclusive_or.hpp"

namespace union_find_detail {
template <typename T>
struct AdditivePotentialGroup : Add<T> {
	static T inverse(const T &value) { return monoid_detail::checked_subtract(T(0), value); }
	static T difference(const T &from, const T &to) { return monoid_detail::checked_subtract(to, from); }

	static bool matches(const T &from, const T &relation, const T &to) {
		if constexpr (monoid_detail::builtin_integer<T> && sizeof(T) <= sizeof(long long)) {
			return __int128(to) - __int128(from) == __int128(relation);
		} else {
			return difference(from, to) == relation;
		}
	}

	static T connection(const T &from, const T &relation, const T &to, bool reverse) {
		if constexpr (monoid_detail::builtin_integer<T> && sizeof(T) <= sizeof(long long)) {
			const __int128 result = __int128(from) + __int128(relation) - __int128(to);
			return monoid_detail::checked_count<T>(reverse ? -result : result);
		} else {
			if (reverse) return difference(Add<T>::combine(from, relation), to);
			return difference(to, Add<T>::combine(from, relation));
		}
	}
};

template <typename T>
struct XorPotentialGroup : ExclusiveOr<T> {
	static T inverse(T value) { return value; }
};
}  // namespace union_find_detail
