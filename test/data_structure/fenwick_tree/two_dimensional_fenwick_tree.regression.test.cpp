#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>
#include "data_structure/fenwick_tree/two_dimensional_fenwick_tree.hpp"
#include "data_structure/two_dimensional_cumulative_sum.hpp"
#include "test/detail/library_checker_aplusb.hpp"

int main() {
	TwoDimensionalFenwickTree<long long> empty;
	assert(empty.empty() && empty.sum(0, 0) == 0);
	TwoDimensionalFenwickTree<long long> zero_width(std::vector<std::vector<long long>>{{}, {999}});
	assert(zero_width.empty() && zero_width.height() == 1 && zero_width.width() == 0);
	assert(empty.query(1, 1, 0, 0) == 0);

	std::mt19937 random(74819);
	for (int height = 1; height <= 6; ++height) {
		for (int width = 1; width <= 8; ++width) {
			std::vector<std::vector<long long>> values(height + 1, std::vector<long long>(width + 1, 999));
			for (int row = 1; row <= height; ++row) {
				for (int column = 1; column <= width; ++column) {
					values[row][column] = int(random() % 31) - 15;
				}
			}
			TwoDimensionalFenwickTree<long long> tree(values);
			assert(tree.get(1, 1) == values[1][1]);
			values[height][width] -= 7;
			tree.set(height, width, values[height][width]);
			TwoDimensionalCumulativeSum<long long> prefix(values);
			for (int top = 1; top <= height; ++top) {
				for (int bottom = top; bottom <= height; ++bottom) {
					for (int left = 1; left <= width; ++left) {
						for (int right = left; right <= width; ++right) {
							long long expected = 0;
							for (int row = top; row <= bottom; ++row) {
								for (int column = left; column <= right; ++column) expected += values[row][column];
							}
							assert(tree.sum(top, left, bottom, right) == expected);
							assert(tree.query(top, left, bottom, right) == expected);
							assert(prefix.query(top, left, bottom, right) == expected);
						}
					}
				}
			}
			assert(tree.sum(0, width) == 0 && tree.sum(height, 0) == 0);
			assert(tree.query(2, 1, 1, width) == 0 && tree.query(1, 2, height, 1) == 0);
		}
	}
	answer_library_checker_aplusb();
}
