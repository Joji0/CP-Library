#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <optional>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include "data_structure/segment_tree/segment_tree.hpp"
#include "graph/flow/dynamic_minimum_cost_path_circulation.hpp"
#include "monoid/clamped_movement_composition.hpp"
#include "monoid/dyadic_affine_composition.hpp"
#include "monoid/grid_boundary_connectivity.hpp"
#include "test/detail/library_checker_aplusb.hpp"

void test_functions(std::mt19937 &random) {
	using Move = ClampedMovementComposition;
	using Half = DyadicAffineComposition<-18, 18>;
	for (int repetition = 0; repetition < 250; ++repetition) {
		const int bound = 1 + random() % 39;
		std::array<Move::value_type, 3> movement;
		std::array<Half::value_type, 3> halves;
		std::array<int, 3> delta, offset;
		std::array<bool, 3> negate;
		for (int i = 0; i < 3; ++i) {
			delta[i] = int(random() % 101) - 50;
			movement[i] = Move::make_shift(delta[i], 0, bound);
			offset[i] = int(random() % 63) - 31;
			negate[i] = random() & 1;
			halves[i] = Half::make_half(offset[i], negate[i]);
		}
		const auto left = Move::combine(Move::combine(movement[0], movement[1]), movement[2]);
		assert(left == Move::combine(movement[0], Move::combine(movement[1], movement[2])));
		assert(Move::combine(left, Move::identity()) == left);
		for (long long start = 0; start <= bound; ++start) {
			long long position = start, distance = 0;
			for (long long shift : delta) {
				const long long next = std::clamp(position + shift, 0LL, static_cast<long long>(bound));
				distance += std::abs(next - position);
				position = next;
			}
			assert((Move::evaluate(left, start) == Move::Movement{position, distance}));
		}
		const auto partial = Half::combine(Half::combine(halves[0], halves[1]), halves[2]);
		assert(partial == Half::combine(halves[0], Half::combine(halves[1], halves[2])));
		assert(Half::combine(partial, Half::identity()) == partial);
		std::optional<long long> fixed;
		for (long long start = -18; start <= 18; ++start) {
			std::optional<long long> expected = start;
			for (int i = 0; i < 3; ++i) {
				const long long numerator = offset[i] + (negate[i] ? -*expected : *expected);
				if ((numerator % 2) || std::abs(numerator / 2) > 18) {
					expected.reset();
					break;
				}
				expected = numerator / 2;
			}
			assert(Half::evaluate(partial, start) == expected);
			if (expected == start) fixed = start;
		}
		assert(Half::fixed_point(partial) == fixed);
	}

	using Wide = DyadicAffineComposition<-1000000000, 2000000000>;
	auto isolated = Wide::identity();
	for (int i = 0; i < 170; ++i) isolated = Wide::combine(isolated, Wide::make_half(3000000000LL, true));
	assert(isolated.first_input == 1000000000 && isolated.last_input == 1000000000);
	assert(Wide::fixed_point(isolated) == 1000000000);
}

void test_grid(std::mt19937 &random) {
	using Grid = GridBoundaryConnectivity<10>;
	const int n = 8, w = 5;
	std::vector<std::string> rows(n + 1, std::string(w + 1, '0'));
	SegmentTree<Grid> tree(n);
	auto fill = [&](int row) {
		for (int col = 1; col <= w; ++col) rows[row][col] = char('0' + random() % 3);
		tree.set(row, Grid::make_leaf(rows[row]));
	};
	for (int i = 1; i <= n; ++i) fill(i);
	for (int q = 0; q < 600; ++q) {
		fill(1 + random() % n);
		int l = 1 + random() % n, r = 1 + random() % n;
		if (l > r) std::swap(l, r);
		const int left = 1 + random() % w, right = 1 + random() % w;
		bool visited[n + 1][w + 1] = {};
		std::vector<std::pair<int, int>> queue{{l, left}};
		visited[l][left] = true;
		for (std::size_t i = 0; i < queue.size(); ++i) {
			const auto [row, col] = queue[i];
			for (auto [dr, dc] : std::array<std::pair<int, int>, 4>{{{0, -1}, {0, 1}, {-1, 0}, {1, 0}}}) {
				const int rr = row + dr, cc = col + dc;
				if (rr < 1 || rr > n || cc < 1 || cc > w || visited[rr][cc] || rows[rr][cc] != rows[row][col]) continue;
				visited[rr][cc] = true;
				queue.emplace_back(rr, cc);
			}
		}
		const bool connected = Grid::connected_with_context(tree.query(1, l - 1), tree.query(l, r), tree.query(r + 1, n), left, right);
		assert(connected == visited[r][right]);
	}
}

void test_path_circulation(std::mt19937 &random) {
	const int n = 6;
	for (int trial = 0; trial < 36; ++trial) {
		std::vector<long long> prices(n + 1), bounds(n);
		std::vector<int> capacity(n + 1);
		for (int i = 1; i <= n; ++i) {
			prices[i] = int(random() % 11) - 5;
			capacity[i] = random() % 3;
		}
		for (int edge = 1; edge < n; ++edge) bounds[edge] = random() % 5;
		DynamicMinimumCostPathCirculation solver(prices, capacity, bounds);
		auto minimum = [&](auto &&self, int vertex, long long prefix, long long cost, long long &best) -> void {
			if (vertex > n) {
				if (prefix == 0) best = std::min(best, cost);
				return;
			}
			for (int net = -capacity[vertex]; net <= capacity[vertex]; ++net) {
				if (vertex < n && std::abs(prefix + net) > bounds[vertex]) continue;
				self(self, vertex + 1, prefix + net, cost - net * prices[vertex], best);
			}
		};
		for (int q = 0; q < 45; ++q) {
			long long expected = 0;
			minimum(minimum, 1, 0, 0, expected);
			assert(solver.minimum_cost() == expected);
			const int vertex = 1 + random() % n;
			if (random() & 1)
				solver.set_price(vertex, prices[vertex] = int(random() % 41) - 20);
			else
				solver.set_vertex_capacity(vertex, capacity[vertex] = random() % 3);
		}
	}
}

int main() {
	std::mt19937 random(278622);
	test_functions(random);
	test_grid(random);
	test_path_circulation(random);
	answer_library_checker_aplusb();
}
