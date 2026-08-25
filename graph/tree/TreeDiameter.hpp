#pragma once
#include <algorithm>
#include <vector>
#include <tuple>
#include "graph/Graph.hpp"

template <typename T>
class TreeDiameter {
public:
        T diameter;
        int u, v;
        std::vector<int> path_nodes;
        std::vector<Edge<T>> path_edges;
        TreeDiameter(const Graph<T>& G) {
                int n = G.size();
                if (n == 0) {
                        diameter = T(0);
                        u = v = -1;
                        return;
                }
                if (n == 1) {
                        diameter = T(0);
                        u = v = 0;
                        path_nodes = {0};
                        return;
                }
                auto bfs = [&](int source) {
                        std::vector<T> dist(n, T(0));
                        std::vector<int> parent_node(n, -1);
                        std::vector<Edge<T>> parent_edge(n);
                        std::vector<int> q;
                        q.reserve(n);
                        q.push_back(source);
                        int head = 0;
                        int farthest = source;
                        T max_dist = T(0);
                        while (head < (int)q.size()) {
                                int curr = q[head++];
                                if (dist[curr] >= max_dist) {
                                        max_dist = dist[curr];
                                        farthest = curr;
                                }
                                for (const auto& e : G[curr]) {
                                        int nxt = e.to;
                                        if (nxt == parent_node[curr]) continue;
                                        dist[nxt] = dist[curr] + e.cost;
                                        parent_node[nxt] = curr;
                                        parent_edge[nxt] = e;
                                        q.emplace_back(nxt);
                                }
                        }
                        return std::make_tuple(farthest, max_dist, parent_node, parent_edge, dist);
                };
                auto [u_node, d1, p_node1, p_edge1, dist1] = bfs(0);
                auto [v_node, max, p_node2, p_edge2, dist2] = bfs(u_node);
                diameter = max;
                u = u_node;
                v = v_node;
                int curr = v;
                while (curr != u) {
                        path_nodes.emplace_back(curr);
                        path_edges.emplace_back(p_edge2[curr]);
                        curr = p_node2[curr];
                }
                path_nodes.emplace_back(u);
                std::reverse(path_nodes.begin(), path_nodes.end());
                std::reverse(path_edges.begin(), path_edges.end());
        }
        std::pair<std::vector<int>, std::vector<int>> dist_from_endpoints(const Graph<T>& G) {
                auto dfs = [&](auto& dfs, int v, int p, std::vector<int>& d) -> void {
                        for (int u : G[v]) {
                                if (u == p) continue;
                                d[u] = d[v] + 1;
                                dfs(dfs, u, v, d);
                        }
                };
                std::vector<int> dist_from_u(G.size(), 0), dist_from_v(G.size(), 0);
                dfs(dfs, u, -1, dist_from_u);
                dfs(dfs, v, -1, dist_from_v);
                return {dist_from_u, dist_from_v};
        };
};
