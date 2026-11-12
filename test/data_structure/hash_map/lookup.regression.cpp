#include <cassert>
#include <string>
#include "data_structure/hash_map.hpp"

int main() {
	HashMap<long long, std::string> map(2);
	const auto &view = map;
	assert(view.empty() && !view.contains(-1) && view.find(-1) == nullptr);
	map[-1] = "";
	const std::string *existing = view.find(-1);
	assert(existing && existing->empty() && view.get(-1, "missing").empty());
	map[-1] = "updated";
	assert(view.find(-1) == existing && *existing == "updated");
	map.build(3);
	assert(!view.contains(-1) && view.get(-1, "missing") == "missing");
	for (int i = -100; i <= 100; ++i) map.insert(i, std::to_string(i));
	assert(view.size() == 201);
	for (int i = -100; i <= 100; ++i) assert(view.get(i) == std::to_string(i));
	map.clear();
	assert(view.empty() && view.find(100) == nullptr);
}
