#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <cassert>
#include <string>
#include <vector>
#include "data_structure/sparse_table/disjoint_sparse_table.hpp"
#include "monoid/add.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
struct Concatenate {
	using value_type = std::string;
	static value_type identity() { return {}; }
	static value_type combine(const value_type &first, const value_type &second) { return first + second; }
};
}  // namespace

int main() {
	DisjointSparseTable<Concatenate> strings;
	DisjointSparseTable<Add<long long>> sums;
	for (int size = 0; size <= 80; ++size) {
		std::vector<std::string> text(size + 1, "dummy");
		std::vector<long long> numbers(size + 1, 999);
		for (int position = 1; position <= size; ++position) {
			text[position] = std::to_string(position) + "|";
			numbers[position] = (position % 2 == 0 ? -1 : 1) * position;
		}
		strings.initialize(text);
		sums.initialize(numbers);
		assert(strings.size() == size && sums.empty() == (size == 0));
		assert(strings.query(1, 0).empty() && sums.product(2, 1) == 0);
		for (int left = 1; left <= size; ++left) {
			assert(strings.get(left) == text[left] && sums.get(left) == numbers[left]);
			std::string expected;
			long long expected_sum = 0;
			for (int right = left; right <= size; ++right) {
				expected += text[right];
				expected_sum += numbers[right];
				assert(strings.query(left, right) == expected);
				assert(sums.product(left, right) == expected_sum);
			}
		}
		assert(strings.all_product() == strings.query(1, size));
		assert(sums.all_product() == sums.query(1, size));
	}
	strings.initialize(std::vector<std::string>{""});
	assert(strings.empty() && strings.all_product().empty());
	answer_library_checker_aplusb();
}
