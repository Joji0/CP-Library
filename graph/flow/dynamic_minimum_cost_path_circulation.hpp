#pragma once
#include <algorithm>
#include <cassert>
#include <limits>
#include <utility>
#include <vector>
#include "acted_monoid/minimum_add.hpp"
#include "data_structure/segment_tree/lazy_segment_tree.hpp"
#include "data_structure/segment_tree/segment_tree.hpp"
#include "monoid/maximum_index.hpp"
#include "monoid/minimum_index.hpp"

// https://atcoder.jp/contests/aalc001/tasks/aalc001_x
// Path edges have symmetric capacities and zero cost. A hub supplies or
// withdraws at each vertex, up to its vertex capacity, at cost -price or price
// per unit respectively. Maintains an optimal circulation as prices change.
class DynamicMinimumCostPathCirculation {
	using Residual = MinimumAdd<long long>;
	using GreatestPrice = MaximumIndex<long long>;
	using LeastPrice = MinimumIndex<long long>;

	int n;
	std::vector<long long> prices;
	std::vector<int> capacity, balance;
	LazySegmentTree<Residual> rightward, leftward;
	SegmentTree<GreatestPrice> can_supply;
	SegmentTree<LeastPrice> can_withdraw;
	__int128 cost = 0;

	static bool positive(const Residual::Value &minimum) { return !minimum || *minimum > 0; }

	void refresh(int vertex) {
		can_supply.set(vertex, balance[vertex] < capacity[vertex] ? GreatestPrice::value_type{prices[vertex], vertex} : GreatestPrice::identity());
		can_withdraw.set(vertex, -capacity[vertex] < balance[vertex] ? LeastPrice::value_type{prices[vertex], vertex} : LeastPrice::identity());
	}

	int best_supplier_to(int vertex) {
		const int left = rightward.min_left(vertex - 1, positive);
		const int right = leftward.max_right(vertex, positive) + 1;
		return GreatestPrice::combine(can_supply.query(left, vertex - 1), can_supply.query(vertex + 1, right)).idx;
	}
	int best_receiver_from(int vertex) {
		const int left = leftward.min_left(vertex - 1, positive);
		const int right = rightward.max_right(vertex, positive) + 1;
		return LeastPrice::combine(can_withdraw.query(left, vertex - 1), can_withdraw.query(vertex + 1, right)).idx;
	}

	void send_one(int source, int target) {
		++balance[source];
		--balance[target];
		if (source < target) {
			rightward.update(source, target - 1, -1);
			leftward.update(source, target - 1, 1);
		} else {
			leftward.update(target, source - 1, -1);
			rightward.update(target, source - 1, 1);
		}
		cost = monoid_detail::checked_add(cost, __int128(prices[target]) - prices[source]);
		refresh(source);
		refresh(target);
	}

	void expand(int vertex) {
		// A direction already available in the old residual graph cannot improve the old optimum.
		const int receiver = balance[vertex] == capacity[vertex] ? best_receiver_from(vertex) : -1;
		const int supplier = balance[vertex] == -capacity[vertex] ? best_supplier_to(vertex) : -1;
		const __int128 outgoing = receiver == -1 ? 0 : __int128(prices[receiver]) - prices[vertex];
		const __int128 incoming = supplier == -1 ? 0 : __int128(prices[vertex]) - prices[supplier];
		++capacity[vertex];
		if (outgoing < 0 && outgoing <= incoming)
			send_one(vertex, receiver);
		else if (incoming < 0)
			send_one(supplier, vertex);
		else
			refresh(vertex);
	}
	void shrink(int vertex) {
		const int bound = --capacity[vertex];
		if (balance[vertex] > bound) {
			const int supplier = best_supplier_to(vertex);
			assert(supplier != -1);
			send_one(supplier, vertex);
		} else if (balance[vertex] < -bound) {
			const int receiver = best_receiver_from(vertex);
			assert(receiver != -1);
			send_one(vertex, receiver);
		} else {
			refresh(vertex);
		}
	}

       public:
	// All vectors are 1-based. path_capacity[i] connects vertices i and i+1.
	// Build O(n + sum(vertex_capacity)*log n). Path capacities must fit in
	// long long even when doubled. Intended for small vertex capacities.
	DynamicMinimumCostPathCirculation(const std::vector<long long> &prices,
					  const std::vector<int> &vertex_capacity, const std::vector<long long> &path_capacity)
	    : n(int(prices.size()) - 1), prices(prices), capacity(prices.size()), balance(prices.size()), rightward(path_capacity), leftward(path_capacity), can_supply(n), can_withdraw(n) {
		assert(n >= 1 && vertex_capacity.size() == prices.size() && path_capacity.size() + 1 == prices.size());
		for (int edge = 1; edge < n; ++edge) {
			assert(0 <= path_capacity[edge] && path_capacity[edge] <= std::numeric_limits<long long>::max() / 2);
		}
		for (int vertex = 1; vertex <= n; ++vertex) set_vertex_capacity(vertex, vertex_capacity[vertex]);
	}

	// O(abs(new_capacity - old_capacity) * log n).
	void set_vertex_capacity(int vertex, int new_capacity) {
		assert(1 <= vertex && vertex <= n && new_capacity >= 0);
		while (capacity[vertex] < new_capacity) expand(vertex);
		while (capacity[vertex] > new_capacity) shrink(vertex);
	}
	// O(vertex_capacity[vertex] * log n).
	void set_price(int vertex, long long price) {
		assert(1 <= vertex && vertex <= n);
		if (prices[vertex] == price) return;
		const int saved_capacity = capacity[vertex];
		set_vertex_capacity(vertex, 0);
		prices[vertex] = price;
		set_vertex_capacity(vertex, saved_capacity);
	}
	long long minimum_cost() const { return monoid_detail::checked_count<long long>(cost); }
};
