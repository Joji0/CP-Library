#pragma once
#include <algorithm>
#include <cassert>
#include <iterator>
#include <numeric>
#include <optional>
#include <vector>

// Static queries on 1-based inclusive ranges. Building costs O(n log n).
template <typename T>
class MergeSortTree {
	using Values = std::vector<T>;
	int n = 0;
	std::vector<Values> sorted_values;

	void build(const Values &values, std::size_t node, int left, int right) {
		if (left == right) {
			sorted_values[node].push_back(values[left]);
			return;
		}
		const int middle = std::midpoint(left, right);
		build(values, node * 2, left, middle);
		build(values, node * 2 + 1, middle + 1, right);
		const auto &first = sorted_values[node * 2];
		const auto &second = sorted_values[node * 2 + 1];
		auto &result = sorted_values[node];
		result.reserve(first.size() + second.size());
		std::merge(first.begin(), first.end(), second.begin(), second.end(), std::back_inserter(result));
	}

	template <typename Visit>
	void visit_range(std::size_t node, int left, int right, int query_left, int query_right, const Visit &visit) const {
		if (query_left <= left && right <= query_right) {
			visit(sorted_values[node]);
			return;
		}
		const int middle = std::midpoint(left, right);
		if (query_left <= middle) visit_range(node * 2, left, middle, query_left, query_right, visit);
		if (middle < query_right) visit_range(node * 2 + 1, middle + 1, right, query_left, query_right, visit);
	}

	template <typename Visit>
	void for_each_range_node(int left, int right, const Visit &visit) const {
		if (left > right) return;
		assert(1 <= left && right <= n);
		visit_range(1, 1, n, left, right, visit);
	}

	template <typename Count>
	int count(int left, int right, const Count &within_node) const {
		int result = 0;
		for_each_range_node(left, right, [&](const Values &values) { result += int(within_node(values)); });
		return result;
	}

       public:
	MergeSortTree() = default;
	MergeSortTree(int size) : n(size) {
		assert(size >= 0);
		sorted_values.resize(std::size_t(size) * 4);
	}
	MergeSortTree(const Values &values) : MergeSortTree(values.empty() ? 0 : int(values.size() - 1)) {
		assert(!values.empty());
		if (n > 0) build(values, 1, 1, n);
	}

	int size() const { return n; }
	bool empty() const { return n == 0; }

	int count_greater_equal(int left, int right, const T &value) const {
		return count(left, right, [&](const Values &values) { return values.end() - std::lower_bound(values.begin(), values.end(), value); });
	}
	int count_less_equal(int left, int right, const T &value) const {
		return count(left, right, [&](const Values &values) { return std::upper_bound(values.begin(), values.end(), value) - values.begin(); });
	}
	int count_greater(int left, int right, const T &value) const {
		return count(left, right, [&](const Values &values) { return values.end() - std::upper_bound(values.begin(), values.end(), value); });
	}
	int count_less(int left, int right, const T &value) const {
		return count(left, right, [&](const Values &values) { return std::lower_bound(values.begin(), values.end(), value) - values.begin(); });
	}
	int count_equal(int left, int right, const T &value) const {
		return count(left, right, [&](const Values &values) {
			const auto [begin, end] = std::equal_range(values.begin(), values.end(), value);
			return end - begin;
		});
	}
	int count_in_value_range(int left, int right, const T &lowest, const T &highest) const {
		if (highest < lowest) return 0;
		return count(left, right, [&](const Values &values) {
			return std::upper_bound(values.begin(), values.end(), highest) - std::lower_bound(values.begin(), values.end(), lowest);
		});
	}

	// The largest value at most bound in [left, right].
	std::optional<T> try_predecessor(int left, int right, const T &bound) const {
		std::optional<T> result;
		for_each_range_node(left, right, [&](const Values &values) {
			const auto after = std::upper_bound(values.begin(), values.end(), bound);
			if (after == values.begin()) return;
			const T &candidate = *std::prev(after);
			if (!result || *result < candidate) result = candidate;
		});
		return result;
	}

	// The smallest value at least bound in [left, right].
	std::optional<T> try_successor(int left, int right, const T &bound) const {
		std::optional<T> result;
		for_each_range_node(left, right, [&](const Values &values) {
			const auto candidate = std::lower_bound(values.begin(), values.end(), bound);
			if (candidate != values.end() && (!result || *candidate < *result)) result = *candidate;
		});
		return result;
	}
};
