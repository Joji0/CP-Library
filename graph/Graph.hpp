#pragma once
#include <vector>
#include <cassert>
#include <utility>

template <typename T = int>
struct Edge {
        int from, to;
        T cost;
        int id;
        Edge() : from(-1), to(-1), cost(0), id(-1) {}
        Edge(int _from, int _to, const T& _cost = 1, int _id = -1)
                : from(_from), to(_to), cost(_cost), id(_id) {}
        Edge(int _from, int _to, T&& _cost, int _id = -1)
                : from(_from), to(_to), cost(std::move(_cost)), id(_id) {}
        operator int() const { return to; }
        friend bool operator<(const Edge<T>& lhs, const Edge<T>& rhs) {
                return lhs.cost < rhs.cost;
        }
        friend bool operator>(const Edge<T>& lhs, const Edge<T>& rhs) {
                return lhs.cost > rhs.cost;
        }
};

template <typename T = int>
using Edges  = std::vector<Edge<T>>;

template <typename T = int>
using GMatrix = std::vector<std::vector<T>>;

template <typename T = int>
struct Graph : std::vector<std::vector<Edge<T>>> {
        using Base = std::vector<std::vector<Edge<T>>>;
        using Base::Base;
        int eid = 0;
        int edge_size() const { return eid; }
        int add_edge(int a, int b, const T& c = 1, bool is_directed = false) {
                assert(0 <= a && a < (int)this->size());
                assert(0 <= b && b < (int)this->size());
                (*this)[a].emplace_back(a, b, c, eid);
                if (!is_directed) {
                        (*this)[b].emplace_back(b, a, c, eid);
                }
                return eid++;
        }
};
