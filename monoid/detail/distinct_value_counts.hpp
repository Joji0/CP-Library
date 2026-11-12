#pragma once
#include <array>
#include <cstddef>
#include <optional>
#include "monoid/detail/checked_arithmetic.hpp"

namespace monoid_detail {
template <typename T, int Capacity, bool Largest>
struct DistinctValueCounts {
	static_assert(Capacity > 0);
	struct Entry {
		T value{};
		long long count = 0;
		bool operator==(const Entry &) const = default;
	};
	struct value_type {
		std::array<Entry, static_cast<std::size_t>(Capacity) + 1> entries{};
		int size = 0;

		std::optional<Entry> kth(int rank) const {
			if (rank < 1 || rank > size) return std::nullopt;
			return entries[rank];
		}
		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_leaf(const T &value) {
		value_type result;
		result.entries[1] = {value, 1};
		result.size = 1;
		return result;
	}

	static value_type combine(const value_type &left, const value_type &right) {
		value_type result;
		std::size_t i = 1, j = 1;
		const auto left_size = static_cast<std::size_t>(left.size);
		const auto right_size = static_cast<std::size_t>(right.size);
		while (result.size < Capacity && (i <= left_size || j <= right_size)) {
			if (j > right_size || (i <= left_size && better(left.entries[i].value, right.entries[j].value))) {
				result.entries[++result.size] = left.entries[i++];
			} else if (i > left_size || better(right.entries[j].value, left.entries[i].value)) {
				result.entries[++result.size] = right.entries[j++];
			} else {
				result.entries[++result.size] = {left.entries[i].value, checked_add(left.entries[i].count, right.entries[j].count)};
				++i;
				++j;
			}
		}
		return result;
	}

       private:
	static bool better(const T &left, const T &right) {
		if constexpr (Largest)
			return right < left;
		else
			return left < right;
	}
};
}  // namespace monoid_detail
