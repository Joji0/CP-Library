#include <algorithm>
#include <cassert>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include "acted_monoid/sum_add.hpp"
#include "data_structure/treap/implicit_treap.hpp"

namespace {
struct TextAction {
	using value_type = std::string;
	struct operator_type {
		int multiplier, offset;
	};

	static std::string id() { return {}; }
	static operator_type op_id() { return {1, 0}; }
	static std::string combine(const std::string &left, const std::string &right) { return left + right; }

	static char map_character(char value, const operator_type &action) {
		return char('a' + ((value - 'a') * action.multiplier + action.offset) % 7);
	}

	static std::string mapping(std::string value, const operator_type &action) {
		for (char &character : value) character = map_character(character, action);
		return value;
	}

	static operator_type op_comp(const operator_type &later, const operator_type &earlier) {
		return {later.multiplier * earlier.multiplier % 7,
			(later.multiplier * earlier.offset + later.offset) % 7};
	}
};

void verify(const ImplicitTreap<TextAction> &sequence, const std::string &expected) {
	assert(sequence.size() == int(expected.size()) && sequence.empty() == expected.empty());
	assert(sequence.all_product() == expected);
	const auto values = sequence.to_vector();
	assert(values.size() == expected.size() + 1 && values[0].empty());
	for (std::size_t i = 1; i < values.size(); ++i) assert(values[i] == expected.substr(i - 1, 1));
}
}  // namespace

int main() {
	std::mt19937 random(52907);
	for (int trial = 0; trial < 50; ++trial) {
		std::vector<std::string> initial{"ignored", "a", "b", "c", "d", "e"};
		ImplicitTreap<TextAction> sequence(initial);
		std::string expected = "abcde";
		for (int step = 0; step < 600; ++step) {
			const int size = int(expected.size());
			int operation = random() % 9;
			if (size == 0) operation = 0;
			if (size >= 60 && operation == 0) operation = 1;
			const char character = char('a' + random() % 7);
			if (operation == 0) {
				const int position = random() % (size + 1);
				sequence.insert(position + 1, std::string(1, character));
				expected.insert(expected.begin() + position, character);
			} else if (operation == 1) {
				const int position = random() % size;
				sequence.erase(position + 1);
				expected.erase(expected.begin() + position);
			} else if (operation == 2) {
				const int position = random() % size;
				sequence.set(position + 1, std::string(1, character));
				expected[position] = character;
				assert(sequence.get(position + 1) == std::string(1, character));
			} else if (operation == 3) {
				const int position = random() % (size + 1);
				auto suffix = sequence.split_off(position + 1);
				verify(sequence, expected.substr(0, position));
				verify(suffix, expected.substr(position));
				sequence.append(std::move(suffix));
				assert(suffix.empty());
			} else if (operation == 4) {
				ImplicitTreap<TextAction> copy(sequence);
				verify(copy, expected);
				copy.set(1, "g");
				verify(sequence, expected);
				ImplicitTreap<TextAction> assigned;
				assigned = sequence;
				verify(assigned, expected);
				sequence = std::move(assigned);
				assert(assigned.empty());
			} else if (operation == 5) {
				if (random() % 2 == 0) {
					assert(sequence.pop_front() == expected.substr(0, 1));
					expected.erase(expected.begin());
				} else {
					assert(sequence.pop_back() == expected.substr(size - 1, 1));
					expected.pop_back();
				}
			} else {
				int left = random() % size, right = random() % size;
				if (left > right) std::swap(left, right);
				if (operation == 6) {
					sequence.reverse(left + 1, right + 1);
					std::reverse(expected.begin() + left, expected.begin() + right + 1);
				} else if (operation == 7) {
					TextAction::operator_type action{int(random() % 7), int(random() % 7)};
					sequence.apply(left + 1, right + 1, action);
					for (int i = left; i <= right; ++i) expected[i] = TextAction::map_character(expected[i], action);
				} else {
					assert(sequence.product(left + 1, right + 1) == expected.substr(left, right - left + 1));
				}
			}
			verify(sequence, expected);
		}
	}

	using Sum = SumAdd<int>;
	auto repeated = ImplicitTreap<Sum>::from_values(std::vector<int>{999});
	assert(!repeated.pop_front() && !repeated.pop_back());
	assert(repeated.product(1, 0).val == 0);
	auto one = ImplicitTreap<Sum>::from_values(std::vector<int>{999, 2});
	for (int i = 0; i < 20000; ++i) repeated.append(one);
	assert(repeated.size() == 20000 && repeated.all_product().val == 40000);
	repeated.apply(1, repeated.size(), 2);
	repeated.set_value(1, 3);
	repeated.insert_value(2, 7);
	repeated.push_front(Sum::make_leaf(1));
	repeated.push_back(Sum::make_leaf(2));
	assert(repeated.pop_front()->val == 1 && repeated.pop_back()->val == 2);
	assert(repeated.product(1, 3).val == 14);
	one.append(one);
	assert(one.size() == 2 && one.all_product().val == 4);
	repeated.clear();
	assert(repeated.empty());
}
