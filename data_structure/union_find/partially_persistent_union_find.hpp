#pragma once
#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

// Updates extend the latest version. Queries may access any version from 1 onward.
class PartiallyPersistentUnionFind {
	std::vector<int> parent, parent_version, current_size;
	std::vector<std::vector<std::pair<int, int>>> size_history;
	std::vector<int> component_history;

	void check(int version, int vertex) const {
		assert(1 <= version && version <= latest_version());
		assert(1 <= vertex && std::size_t(vertex) < parent.size());
	}

       public:
	explicit PartiallyPersistentUnionFind(int size = 0)
	    : parent(std::size_t(size) + 1), parent_version(parent.size()), current_size(parent.size(), 1), size_history(parent.size()), component_history{0, size} {
		assert(size >= 0);
	}
	int latest_version() const { return int(component_history.size() - 1); }
	int count(int version) const {
		assert(1 <= version && version <= latest_version());
		return component_history[version];
	}
	int find(int version, int vertex) const {
		check(version, vertex);
		while (parent_version[vertex] != 0 && parent_version[vertex] <= version) vertex = parent[vertex];
		return vertex;
	}
	bool same(int version, int first, int second) const {
		return find(version, first) == find(version, second);
	}
	int size(int version, int vertex) const {
		const auto &history = size_history[find(version, vertex)];
		const auto next = std::upper_bound(history.begin(), history.end(), version,
						   [](int time, const auto &entry) { return time < entry.first; });
		return next == history.begin() ? 1 : (next - 1)->second;
	}

	int merge(int first, int second) {
		const int previous = latest_version();
		first = find(previous, first);
		second = find(previous, second);
		const int version = previous + 1;
		const int components = count(previous);
		component_history.push_back(components - (first != second));
		if (first != second) {
			if (current_size[first] < current_size[second]) std::swap(first, second);
			const int merged_size = current_size[first] + current_size[second];
			try {
				size_history[first].emplace_back(version, merged_size);
			} catch (...) {
				component_history.pop_back();
				throw;
			}
			parent[second] = first;
			parent_version[second] = version;
			current_size[first] = merged_size;
		}
		return version;
	}
};
