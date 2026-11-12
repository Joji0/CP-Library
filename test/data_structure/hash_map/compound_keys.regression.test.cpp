#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"

#include <array>
#include <cassert>
#include <limits>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>
#include "data_structure/hash_map.hpp"
#include "test/detail/library_checker_aplusb.hpp"

namespace {
struct Vertex {
	int id;
	std::string label;
	std::vector<int> neighbors;

	bool operator==(const Vertex &) const = default;
	auto hash_fields() const { return std::tie(id, label, neighbors); }
};

struct Key {
	explicit Key(int group, int detail) : group(group), detail(detail) {}
	int group, detail;
};

struct GroupHasher {
	explicit GroupHasher(int salt) : salt(salt) {}
	std::size_t operator()(const Key &key) const { return std::size_t(key.group) + std::size_t(salt); }
	int salt;
};

struct GroupEqual {
	bool operator()(const Key &first, const Key &second) const { return first.group == second.group; }
};

struct ConstantHasher {
	std::size_t operator()(int) const { return 0; }
};

struct Immovable {
	explicit Immovable(int value) : value(value) {
		if (value < 0) throw std::runtime_error("invalid test value");
	}
	Immovable(const Immovable &) = delete;
	Immovable(Immovable &&) = delete;
	int value;
};

void structural_keys() {
	HashMap<std::string, std::vector<int>> text;
	text["alpha"] = {1, 2};
	text.insert("beta", {3});
	assert((text.get("alpha") == std::vector<int>{1, 2}));
	assert(text.find("beta")->front() == 3 && !text.contains("missing"));

	HashMap<std::pair<int, int>, std::string> pairs(2);
	pairs[{2, 3}] = "forward";
	pairs[{3, 2}] = "backward";
	assert(pairs.get({2, 3}) == "forward" && pairs.get({3, 2}) == "backward");

	using Compound = std::tuple<std::array<int, 2>, std::vector<std::pair<int, int>>,
				    std::optional<std::vector<bool>>, std::variant<int, std::vector<std::string>>>;
	const Compound first{{1, 2}, {{3, 4}, {5, 6}}, std::vector<bool>{false, true}, std::vector<std::string>{"x", "y"}};
	Compound second = first;
	std::get<1>(second)[0] = {4, 3};
	HashMap<Compound, int> compound;
	compound[first] = 21;
	compound[second] = 34;
	assert(compound.size() == 2 && compound.get(first) == 21 && compound.get(second) == 34);

	using Group = std::unordered_map<int, std::vector<std::string>>;
	Group one{{1, {"a"}}, {2, {"b", "c"}}};
	Group two{{2, {"b", "c"}}, {1, {"a"}}};
	two.rehash(40);
	assert(one == two && StructuralHash{}(one) == StructuralHash{}(two));
	HashMap<Group, int> unordered;
	unordered[one] = 8;
	assert(unordered.get(two) == 8);
	two[2].push_back("d");
	assert(!unordered.contains(two));

	HashMap<Vertex, long long> vertices;
	vertices[{7, "north", {2, 3}}] = 100;
	assert(vertices.get({7, "north", {2, 3}}) == 100);
	assert(!vertices.contains({7, "north", {3, 2}}));

	HashMap<__int128, int> wide;
	const __int128 upper_bits = __int128(1) << 100;
	wide[0] = 5;
	wide[upper_bits] = 6;
	wide[-upper_bits] = 7;
	assert(wide.size() == 3 && wide.get(0) == 5 && wide.get(upper_bits) == 6 && wide.get(-upper_bits) == 7);

	HashMap<Key, std::string, GroupHasher, GroupEqual> grouped(2, GroupHasher(123), GroupEqual{});
	grouped.try_emplace(Key(1, 7), "one");
	const auto [same, inserted] = grouped.try_emplace(Key(1, 999), "unused");
	assert(!inserted && *same == "one");
	const auto [updated, new_key] = grouped.insert_or_assign(Key(1, -5), "updated");
	assert(!new_key && updated == same && *same == "updated");
	auto copied = grouped;
	copied.insert_or_assign(Key(1, 2), "copy");
	assert(*grouped.find(Key(1, 0)) == "updated" && copied.get(Key(1, 0)) == "copy");
}

void value_lifetimes() {
	HashMap<int, Immovable> entries(2);
	const auto [stable, inserted] = entries.try_emplace(1, 11);
	assert(inserted && stable->value == 11);
	const auto [unchanged, duplicate] = entries.try_emplace(1, -1);
	assert(!duplicate && unchanged == stable);
	try {
		entries.try_emplace(2, -1);
		assert(false);
	} catch (const std::runtime_error &) {
		assert(entries.size() == 1 && !entries.contains(2) && entries.find(1) == stable);
	}

	for (int key = 2; key <= 300; ++key) entries.try_emplace(key, key);
	entries.reserve(1000);
	assert(entries.find(1) == stable && stable->value == 11);
	assert(entries.erase(7) && !entries.erase(7));
	try {
		entries.try_emplace(8 + 1000, -1);
		assert(false);
	} catch (const std::runtime_error &) {
		assert(entries.size() == 299 && !entries.contains(1008));
	}
	assert(entries.try_emplace(1008, 80).first->value == 80);
	assert(entries.find(1) == stable);
	auto moved = std::move(entries);
	assert(moved.find(1) == stable && entries.empty());
	entries.try_emplace(3, 3);
	assert(entries.find(3)->value == 3 && moved.find(1)->value == 11);

	HashMap<std::string, std::unique_ptr<int>> owned(2);
	owned["slot"] = std::make_unique<int>(19);
	owned.insert_or_assign("other", std::make_unique<int>(27));
	owned.reserve(1000);
	assert(**owned.find("slot") == 19 && **owned.find("other") == 27);
}

void collisions_and_erasure() {
	HashMap<int, int, ConstantHasher> map(2);
	std::unordered_map<int, int> expected;
	std::mt19937 random(9371);
	for (int operation = 0; operation < 6500; ++operation) {
		const int key = int(random() % 191) - 95;
		switch (random() % 4) {
			case 0: {
				const int value = int(random() % 1000);
				map.insert(key, value);
				expected.insert_or_assign(key, value);
				break;
			}
			case 1:
				assert(map.erase(key) == (expected.erase(key) != 0));
				break;
			case 2:
				++map[key];
				++expected[key];
				break;
			case 3: {
				const auto found = expected.find(key);
				assert(map.contains(key) == (found != expected.end()));
				assert(map.get(key, -1) == (found == expected.end() ? -1 : found->second));
				break;
			}
		}
		assert(map.size() == int(expected.size()));
	}
	for (auto [key, value] : expected) assert(map.get(key) == value);
	map.clear();
	assert(map.empty() && !map.find(0));
	map.reserve(20);
	assert(map.bucket_count() >= 40);
	map.build(3);
	assert(map.empty() && map.bucket_count() == 4);
}
}  // namespace

int main() {
	structural_keys();
	value_lifetimes();
	collisions_and_erasure();
	answer_library_checker_aplusb();
}
