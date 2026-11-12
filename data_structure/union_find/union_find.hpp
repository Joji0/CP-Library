#pragma once
#include <algorithm>
#include <cassert>
#include <numeric>
#include <limits>
#include <stdexcept>
#include <vector>

class UnionFind {
	std::vector<int> parent, sz;
	int num_comps;

       public:
	UnionFind(int n = 0) { initialize(n); }
	void initialize(int n) {
		assert(n >= 0);
		parent.resize(std::size_t(n) + 1);
		std::iota(parent.begin(), parent.end(), std::size_t(0));
		sz.assign(std::size_t(n) + 1, 1);
		num_comps = n;
	}
	int find(int x) {
		assert(1 <= x && x <= vertex_count());
		return x == parent[x] ? x : parent[x] = find(parent[x]);
	}
	bool same(int a, int b) { return find(a) == find(b); }
	int size(int x) { return sz[find(x)]; }
	int count() const { return num_comps; }
	int vertex_count() const { return int(parent.size() - 1); }

	int add_vertex() {
		if (vertex_count() == std::numeric_limits<int>::max()) throw std::length_error("UnionFind vertex limit");
		const int id = vertex_count() + 1;
		parent.push_back(id);
		try {
			sz.push_back(1);
		} catch (...) {
			parent.pop_back();
			throw;
		}
		++num_comps;
		return id;
	}

	bool merge(int a, int b) {
		a = find(a), b = find(b);
		if (a != b) {
			if (sz[a] < sz[b]) std::swap(a, b);
			parent[b] = a;
			sz[a] += sz[b];
			num_comps--;
			return true;
		}
		return false;
	}

	bool join(int a, int b) { return merge(a, b); }
	bool unite(int a, int b) { return merge(a, b); }

	std::vector<std::vector<int>> groups() {
		int n = (int)parent.size();
		std::vector<int> group_size(n, 0);
		for (int i = 1; i < n; ++i) group_size[find(i)]++;
		std::vector<std::vector<int>> result;
		std::vector<int> root_map(n, -1);
		for (int i = 1; i < n; ++i) {
			int r = find(i);
			if (root_map[r] == -1) {
				root_map[r] = int(result.size());
				result.push_back({});
				result.back().reserve(group_size[r]);
			}
			result[root_map[r]].push_back(i);
		}
		return result;
	}
};
