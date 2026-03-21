#pragma once

#include <climits>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <functional>
#include <optional>
#include <utility>
using namespace std;
using namespace __gnu_pbds;
template <class T, class Cmp = less<T>> struct OrderedMultiSet {
        using Key = pair<T, int>;
        struct KeyCmp {
                Cmp cmp;
                bool operator()(const Key &a, const Key &b) const {
                        if (cmp(a.first, b.first)) return true;
                        if (cmp(b.first, a.first)) return false;
                        return a.second < b.second;
                }
        };
        using OST = tree<Key, null_type, KeyCmp, rb_tree_tag, tree_order_statistics_node_update>;
        OST tr;
        int uid = 0;
        int size() const { return (int)tr.size(); }
        bool empty() const { return tr.empty(); }
        void clear() {
                tr.clear();
                uid = 0;
        }
        void insert(const T &x) { tr.insert({x, uid++}); }
        bool erase_one(const T &x) {
                auto it = tr.lower_bound({x, INT_MIN});
                if (it == tr.end() || Cmp{}(x, it->first) || Cmp{}(it->first, x)) return false;
                tr.erase(it);
                return true;
        }
        int erase_all(const T &x) {
                int l = order_of_key(x);
                int r = order_of_key_next(x);
                int cnt = r - l;
                while (cnt--) {
                        auto it = tr.lower_bound({x, INT_MIN});
                        tr.erase(it);
                }
                return r - l;
        }
        int order_of_key(const T &x) const { return (int)tr.order_of_key({x, INT_MIN}); }
        int order_of_key_next(const T &x) const { return (int)tr.order_of_key({x, INT_MAX}); }
        int count(const T &x) const { return order_of_key_next(x) - order_of_key(x); }
        optional<T> kth(int k) const {
                if (k < 0 || k >= size()) return nullopt;
                return tr.find_by_order(k)->first;
        }
        int count_halfopen(const T &l, const T &r) const { return order_of_key(r) - order_of_key(l); }
        int count_range(const T &l, const T &r) const {
                if (Cmp{}(r, l)) return 0;
                return order_of_key_next(r) - order_of_key(l);
        }
        optional<T> lower_bound(const T &x) const {
                auto it = tr.lower_bound({x, INT_MIN});
                if (it == tr.end()) return nullopt;
                return it->first;
        }
        optional<T> upper_bound(const T &x) const {
                auto it = tr.upper_bound({x, INT_MAX});
                if (it == tr.end()) return nullopt;
                return it->first;
        }
        optional<T> erase_kth(int k) {
                if (k < 0 || k >= size()) return nullopt;
                auto it = tr.find_by_order(k);
                T val = it->first;
                tr.erase(it);
                return val;
        }
        optional<T> min_value() const {
                if (empty()) return nullopt;
                return tr.begin()->first;
        }
        optional<T> max_value() const {
                if (empty()) return nullopt;
                auto it = tr.end();
                --it;
                return it->first;
        }
};
