#pragma once
#include <algorithm>
#include <numeric>
#include <vector>

struct UnionFind {
        std::vector<int> parent, sz;
        int num_comps;
        UnionFind(int n = 0) { init(n); }
        void init(int n) {
                parent.resize(n);
                iota(parent.begin(), parent.end(), 0);
                sz.assign(n, 1);
                num_comps = n;
        }
        int find(int x) { return (x == parent[x] ? x : parent[x] = find(parent[x])); }
        bool same(int a, int b) { return find(a) == find(b); }
        int size(int x) { return sz[find(x)]; }
        bool join(int a, int b) {
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
        std::vector<std::vector<int>> groups() {
                int n = parent.size();
                std::vector<std::vector<int>> res(n);
                std::vector<int> group_size(n, 0);
                for (int i = 0; i < n; i++) {
                        group_size[find(i)]++;
                }
                std::vector<std::vector<int>> result;
                std::vector<int> root_map(n, -1);
                for (int i = 0; i < n; i++) {
                        int r = find(i);
                        if (root_map[r] == -1) {
                                root_map[r] = result.size();
                                result.push_back({});
                                result.back().reserve(group_size[r]);
                        }
                        result[root_map[r]].push_back(i);
                }
                return result;
        }
};
