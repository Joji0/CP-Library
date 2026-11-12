#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"
#include <iostream>
#include <optional>
#include "data_structure/treap/treap.hpp"

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	int size, queries;
	std::cin >> size >> queries;
	Treap<int> set;
	for (int i = 0; i < size; ++i) {
		int value;
		std::cin >> value;
		if (!set.contains(value)) set.insert(value);
	}
	while (queries--) {
		int operation, value;
		std::cin >> operation >> value;
		if (operation == 0) {
			if (!set.contains(value)) set.insert(value);
		} else if (operation == 1) {
			set.erase_one(value);
		} else if (operation == 3) {
			std::cout << set.count_less_equal(value) << '\n';
		} else {
			std::optional<int> answer;
			if (operation == 2) answer = set.kth(value);
			if (operation == 4) answer = set.max_less_equal(value);
			if (operation == 5) answer = set.lower_bound(value);
			std::cout << answer.value_or(-1) << '\n';
		}
	}
}
