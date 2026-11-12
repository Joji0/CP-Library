#pragma once
#include <type_traits>
#include <algorithm>
#include <bit>
#include <cassert>
#include <limits>
#include <vector>
#include "monoid/detail/checked_arithmetic.hpp"

// Inclusive range chmin, chmax, add, and sum/min/max.
// Logical values and returned sums must be representable in T.
template <typename T = long long>
class SegmentTreeBeats {
	static constexpr bool widen = monoid_detail::builtin_integer<T> && sizeof(T) <= sizeof(long long);
	// Push applies addition before a deferred clamp, so stale child extrema can briefly exceed T.
	using Storage = std::conditional_t<widen, __int128, T>;
	static constexpr Storage positive_infinity = std::numeric_limits<Storage>::max();
	static constexpr Storage negative_infinity = std::numeric_limits<Storage>::lowest();

	struct Node {
		Storage sum = 0;
		Storage maximum = negative_infinity;
		Storage second_maximum = negative_infinity;
		Storage minimum = positive_infinity;
		Storage second_minimum = positive_infinity;
		Storage lazy_add = 0;
		int maximum_count = 0;
		int minimum_count = 0;
		int length = 0;
	};

	static T to_value(const Storage &value) {
		if constexpr (widen)
			return monoid_detail::checked_count<T>(value);
		else
			return value;
	}

	int _n;
	std::vector<Node> _nodes;

	static Node make_leaf(T value) {
		Node node;
		node.sum = value;
		node.maximum = value;
		node.minimum = value;
		node.maximum_count = 1;
		node.minimum_count = 1;
		node.length = 1;
		return node;
	}

	static Node merge_nodes(const Node &left, const Node &right) {
		Node result;
		result.sum = left.sum + right.sum;
		result.length = left.length + right.length;

		result.maximum = std::max(left.maximum, right.maximum);
		result.maximum_count = 0;
		result.second_maximum = negative_infinity;
		if (left.maximum == result.maximum) {
			result.maximum_count += left.maximum_count;
			result.second_maximum = std::max(result.second_maximum, left.second_maximum);
		} else {
			result.second_maximum = std::max(result.second_maximum, left.maximum);
		}
		if (right.maximum == result.maximum) {
			result.maximum_count += right.maximum_count;
			result.second_maximum = std::max(result.second_maximum, right.second_maximum);
		} else {
			result.second_maximum = std::max(result.second_maximum, right.maximum);
		}

		result.minimum = std::min(left.minimum, right.minimum);
		result.minimum_count = 0;
		result.second_minimum = positive_infinity;
		if (left.minimum == result.minimum) {
			result.minimum_count += left.minimum_count;
			result.second_minimum = std::min(result.second_minimum, left.second_minimum);
		} else {
			result.second_minimum = std::min(result.second_minimum, left.minimum);
		}
		if (right.minimum == result.minimum) {
			result.minimum_count += right.minimum_count;
			result.second_minimum = std::min(result.second_minimum, right.second_minimum);
		} else {
			result.second_minimum = std::min(result.second_minimum, right.minimum);
		}
		return result;
	}

	void build(int node, int left, int right, const std::vector<T> &values) {
		if (right - left == 1) {
			_nodes[node] = make_leaf(values[left + 1]);
			return;
		}
		int middle = left + (right - left) / 2;
		build(node * 2, left, middle, values);
		build(node * 2 + 1, middle, right, values);
		pull(node);
	}

	void pull(int node) {
		_nodes[node] = merge_nodes(_nodes[node * 2], _nodes[node * 2 + 1]);
	}

	void apply_add(int node, Storage value) {
		Node &current = _nodes[node];
		current.sum = current.sum + value * Storage(current.length);
		current.maximum += value;
		current.minimum += value;
		if (current.maximum_count != current.length) current.second_maximum += value;
		if (current.minimum_count != current.length) current.second_minimum += value;
		if (current.length > 1) current.lazy_add += value;
	}

	void apply_chmin(int node, Storage value) {
		Node &current = _nodes[node];
		if (current.maximum <= value) return;
		current.sum = current.sum + (value - current.maximum) * Storage(current.maximum_count);
		if (current.minimum == current.maximum) {
			current.minimum = value;
		} else if (current.second_minimum == current.maximum) {
			current.second_minimum = value;
		}
		current.maximum = value;
	}

	void apply_chmax(int node, Storage value) {
		Node &current = _nodes[node];
		if (value <= current.minimum) return;
		current.sum = current.sum + (value - current.minimum) * Storage(current.minimum_count);
		if (current.maximum == current.minimum) {
			current.maximum = value;
		} else if (current.second_maximum == current.minimum) {
			current.second_maximum = value;
		}
		current.minimum = value;
	}

	void push(int node) {
		Node &current = _nodes[node];
		if (current.length == 1) return;
		if (current.lazy_add != 0) {
			apply_add(node * 2, current.lazy_add);
			apply_add(node * 2 + 1, current.lazy_add);
			current.lazy_add = 0;
		}
		apply_chmin(node * 2, current.maximum);
		apply_chmin(node * 2 + 1, current.maximum);
		apply_chmax(node * 2, current.minimum);
		apply_chmax(node * 2 + 1, current.minimum);
	}

	void range_chmin(int node, int left, int right, int query_left, int query_right, T value) {
		Node &current = _nodes[node];
		if (query_right <= left || right <= query_left || current.maximum <= value) return;
		if (query_left <= left && right <= query_right && (current.maximum_count == current.length || current.second_maximum < value)) {
			apply_chmin(node, value);
			return;
		}
		push(node);
		int middle = left + (right - left) / 2;
		range_chmin(node * 2, left, middle, query_left, query_right, value);
		range_chmin(node * 2 + 1, middle, right, query_left, query_right, value);
		pull(node);
	}

	void range_chmax(int node, int left, int right, int query_left, int query_right, T value) {
		Node &current = _nodes[node];
		if (query_right <= left || right <= query_left || value <= current.minimum) return;
		if (query_left <= left && right <= query_right && (current.minimum_count == current.length || value < current.second_minimum)) {
			apply_chmax(node, value);
			return;
		}
		push(node);
		int middle = left + (right - left) / 2;
		range_chmax(node * 2, left, middle, query_left, query_right, value);
		range_chmax(node * 2 + 1, middle, right, query_left, query_right, value);
		pull(node);
	}

	void range_add(int node, int left, int right, int query_left, int query_right, T value) {
		if (query_right <= left || right <= query_left) return;
		if (query_left <= left && right <= query_right) {
			if constexpr (widen) {
				to_value(_nodes[node].minimum + value);
				to_value(_nodes[node].maximum + value);
			}
			apply_add(node, value);
			return;
		}
		push(node);
		int middle = left + (right - left) / 2;
		range_add(node * 2, left, middle, query_left, query_right, value);
		range_add(node * 2 + 1, middle, right, query_left, query_right, value);
		pull(node);
	}

	Node range_query(int node, int left, int right, int query_left, int query_right) {
		if (query_left <= left && right <= query_right) return _nodes[node];
		push(node);
		int middle = left + (right - left) / 2;
		if (query_right <= middle) return range_query(node * 2, left, middle, query_left, query_right);
		if (middle <= query_left) return range_query(node * 2 + 1, middle, right, query_left, query_right);
		return merge_nodes(range_query(node * 2, left, middle, query_left, query_right),
				   range_query(node * 2 + 1, middle, right, query_left, query_right));
	}

       public:
	SegmentTreeBeats() : _n(0) {}
	explicit SegmentTreeBeats(int n) : _n(n) {
		assert(0 <= n);
		if (_n != 0) {
			_nodes.resize(2 * std::bit_ceil(std::size_t(_n)));
			std::vector<T> values(std::size_t(_n) + 1, T(0));
			build(1, 0, _n, values);
		}
	}
	explicit SegmentTreeBeats(const std::vector<T> &values) : _n(int(values.size()) - 1) {
		assert(!values.empty());
		if (_n != 0) {
			_nodes.resize(2 * std::bit_ceil(std::size_t(_n)));
			build(1, 0, _n, values);
		}
	}

	int size() const { return _n; }
	bool empty() const { return _n == 0; }

	// Range chmin: a[i] <- min(a[i], value) for i in [left, right]
	void chmin(int left, int right, T value) {
		if (left > right) return;
		assert(1 <= left && right <= _n);
		range_chmin(1, 0, _n, left - 1, right, value);
	}

	// Range chmax: a[i] <- max(a[i], value) for i in [left, right]
	void chmax(int left, int right, T value) {
		if (left > right) return;
		assert(1 <= left && right <= _n);
		range_chmax(1, 0, _n, left - 1, right, value);
	}

	// Range add: a[i] <- a[i] + value for i in [left, right]
	void add(int left, int right, T value) {
		if (left > right) return;
		assert(1 <= left && right <= _n);
		range_add(1, 0, _n, left - 1, right, value);
	}

	// Query sum on inclusive interval [left, right]
	T sum(int left, int right) {
		if (left > right) return T(0);
		assert(1 <= left && right <= _n);
		return to_value(range_query(1, 0, _n, left - 1, right).sum);
	}

	T min(int left, int right) {
		if (left > right) return std::numeric_limits<T>::max();
		assert(1 <= left && left <= right && right <= _n);
		return to_value(range_query(1, 0, _n, left - 1, right).minimum);
	}

	T max(int left, int right) {
		if (left > right) return std::numeric_limits<T>::lowest();
		assert(1 <= left && left <= right && right <= _n);
		return to_value(range_query(1, 0, _n, left - 1, right).maximum);
	}

	T get(int index) {
		assert(1 <= index && index <= _n);
		return sum(index, index);
	}

	void set(int index, T value) {
		assert(1 <= index && index <= _n);
		chmin(index, index, value);
		chmax(index, index, value);
	}
};
