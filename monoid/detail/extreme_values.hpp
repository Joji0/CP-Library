#pragma once
#include <array>
#include <optional>

namespace monoid_detail {
template <typename T, int Capacity, bool Largest, bool Distinct>
struct ExtremeValues {
	static_assert(Capacity > 0);

	struct value_type {
		std::array<T, static_cast<std::size_t>(Capacity) + 1> values{};
		int count = 0;

		std::optional<T> kth(int rank) const {
			if (rank < 1 || rank > count) return std::nullopt;
			return values[rank];
		}

		bool operator==(const value_type &) const = default;
	};

	static value_type identity() { return {}; }
	static value_type make_leaf(const T &value) {
		value_type result;
		result.values[1] = value;
		result.count = 1;
		return result;
	}

	static value_type combine(const value_type &left, const value_type &right) {
		value_type result;
		std::size_t first = 1, second = 1;
		const auto left_end = static_cast<std::size_t>(left.count);
		const auto right_end = static_cast<std::size_t>(right.count);
		while (result.count < Capacity && (first <= left_end || second <= right_end)) {
			const bool use_left = second > right_end ||
					      (first <= left_end && !better(right.values[second], left.values[first]));
			const T &next = use_left ? left.values[first++] : right.values[second++];
			if constexpr (Distinct) {
				if (result.count != 0 && !better(result.values[result.count], next)) continue;
			}
			result.values[++result.count] = next;
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
