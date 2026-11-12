#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <algorithm>
#include <cassert>
#include <map>
#include <random>
#include <utility>
#include <vector>
#include "data_structure/frequency_summary/verify_heavy_hitters.hpp"
#include "monoid/dynamic_misra_gries.hpp"
#include "monoid/misra_gries.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
template <typename Summary, typename MakeNode>
void check(int capacity, MakeNode make_node) {
	std::mt19937 random(917);
	for (int trial = 0; trial < 500; ++trial) {
		std::map<int, long long> frequencies;
		std::vector<typename Summary::Node> nodes;
		long long size = 0;
		for (int item = 0; item < 75; ++item) {
			const int key = int(random() % 11) - 5;
			const long long weight = random() % 16;
			frequencies[key] += weight;
			size += weight;
			nodes.push_back(make_node(key, weight));
		}
		while (nodes.size() > 1) {
			const std::size_t first = random() % nodes.size();
			auto node = nodes[first];
			nodes.erase(nodes.begin() + first);
			const std::size_t second = random() % nodes.size();
			nodes[second] = Summary::merge(std::move(node), nodes[second]);
		}
		const auto &summary = nodes.front();
		assert(summary.length == size);
		const long long threshold = size / (capacity + 1LL) + 1;
		std::vector<std::pair<int, long long>> expected;
		for (auto [key, count] : frequencies) {
			if (count >= threshold) expected.emplace_back(key, count);
		}
		assert(verify_heavy_hitters(summary, threshold, [&](int key) { return frequencies.at(key); }) == expected);
	}
}
}  // namespace

int main() {
	using Majority = MisraGries<1>;
	const Majority::Node first(1), second(2), third(3);
	assert(Majority::merge(Majority::merge(first, second), third).candidates() !=
	       Majority::merge(first, Majority::merge(second, third)).candidates());
	check<Majority>(1, [](int key, long long count) { return Majority::Node(key, count); });
	using Fixed = MisraGries<4>;
	check<Fixed>(4, [](int key, long long count) { return Fixed::Node(key, count); });
	using Dynamic = DynamicMisraGries<7>;
	for (int capacity = 0; capacity <= 7; ++capacity) {
		check<Dynamic>(capacity, [=](int key, long long count) { return Dynamic::Node(key, capacity, count); });
	}
	answer_library_checker_aplusb();
}
