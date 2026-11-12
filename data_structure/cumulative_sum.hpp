#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
#include "monoid/add.hpp"

namespace cumulative_sum_detail {
template <typename M>
concept Monoid = requires(const typename M::value_type &first, const typename M::value_type &second) {
	M::identity();
	M::combine(first, second);
};

template <typename M>
concept HasPrefixRemoval = requires(const typename M::value_type &whole, const typename M::value_type &prefix) {
	M::remove_prefix(whole, prefix);
};
}  // namespace cumulative_sum_detail

// O(1) prefix products after O(n) build. Prefix values must fit the monoid's value type.
template <typename M, bool = cumulative_sum_detail::Monoid<M>>
class CumulativeSum {
	using T = typename M::value_type;
	int n = 0;
	std::vector<T> data;

       public:
	CumulativeSum() : CumulativeSum(0) {}
	CumulativeSum(int size) : n(size) {
		assert(size >= 0);
		data.assign(std::size_t(size) + 1, M::identity());
	}
	CumulativeSum(const std::vector<T> &values) { build(values); }

	void build(const std::vector<T> &values) {
		assert(!values.empty());
		std::vector<T> next;
		next.reserve(values.size());
		next.push_back(M::identity());
		for (std::size_t i = 1; i < values.size(); ++i) next.push_back(M::combine(next.back(), values[i]));
		data = std::move(next);
		n = int(values.size() - 1);
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }
	T all_product() const { return data.back(); }

	T query_prefix(int right) const {
		assert(0 <= right && right <= n);
		return data[right];
	}
	T prefix_product(int right) const { return query_prefix(right); }

	// Removes the prefix [1, left - 1] from [1, right] to obtain the inclusive range.
	template <bool Enabled = true>
		requires(Enabled && cumulative_sum_detail::HasPrefixRemoval<M>)
	T query(int left, int right) const {
		if (left > right) return M::identity();
		assert(1 <= left && right <= n);
		return M::remove_prefix(data[right], data[left - 1]);
	}

	template <bool Enabled = true>
		requires(Enabled && cumulative_sum_detail::HasPrefixRemoval<M>)
	T product(int left, int right) const {
		return query(left, right);
	}

	const std::vector<T> &get_data() const { return data; }
};

// Raw numeric types use addition.
template <typename T>
class CumulativeSum<T, false> : public CumulativeSum<Add<T>, true> {
	using Base = CumulativeSum<Add<T>, true>;

       public:
	using Base::Base;
};
