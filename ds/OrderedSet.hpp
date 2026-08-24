#pragma once
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <functional>
#include <optional>

template <class T, class Cmp = std::less<T>>
class OrderedSet {
        using OST = __gnu_pbds::tree<T, __gnu_pbds::null_type, Cmp, __gnu_pbds::rb_tree_tag,
                                     __gnu_pbds::tree_order_statistics_node_update>;
        OST tr;
public:
        int size() const { return (int)tr.size(); }
        bool empty() const { return tr.empty(); }
        void clear() { tr.clear(); }
        bool insert(const T& x) {
                auto [it, ok] = tr.insert(x);
                return ok;
        }
        bool erase(const T& x) { return tr.erase(x) > 0; }
        std::optional<T> erase_kth(int k) {
                if (k < 0 || k >= size()) return std::nullopt;
                auto it = tr.find_by_order(k);
                T val = *it;
                tr.erase(it);
                return val;
        }
        bool contains(const T& x) const { return tr.find(x) != tr.end(); }
        int order_of_key(const T& x) const { return (int)tr.order_of_key(x); }
        std::optional<T> kth(int k) const {
                if (k < 0 || k >= size()) return std::nullopt;
                auto it = tr.find_by_order(k);
                return *it;
        }
        std::optional<T> first() const { return kth(0); }
        std::optional<T> last() const { return kth(size() - 1); }
        std::optional<T> lower_bound(const T& x) const {
                auto it = tr.lower_bound(x);
                if (it == tr.end()) return std::nullopt;
                return *it;
        }
        std::optional<T> upper_bound(const T& x) const {
                auto it = tr.upper_bound(x);
                if (it == tr.end()) return std::nullopt;
                return *it;
        }
        std::optional<T> max_less(const T& x) const {
                auto it = tr.lower_bound(x);
                if (it == tr.begin()) return std::nullopt;
                --it;
                return *it;
        }
        std::optional<T> max_less_equal(const T& x) const {
                auto it = tr.upper_bound(x);
                if (it == tr.begin()) return std::nullopt;
                --it;
                return *it;
        }
        std::optional<T> min_greater(const T& x) const {
                return upper_bound(x);
        }
        std::optional<T> min_greater_equal(const T& x) const {
                return lower_bound(x);
        }
        int count_less(const T& x) const {
                return order_of_key(x);
        }
        int count_less_equal(const T& x) const {
                return count_less(x) + (contains(x) ? 1 : 0);
        }
        int count_greater(const T& x) const {
                return size() - count_less_equal(x);
        }
        int count_greater_equal(const T& x) const {
                return size() - count_less(x);
        }
        int count_range(const T& l, const T& r) const {
                if (Cmp{}(r, l)) return 0;
                return count_less_equal(r) - count_less(l);
        }
        int count_halfopen(const T& l, const T& r) const {
                if (Cmp{}(r, l)) return 0;
                return count_less(r) - count_less(l);
        }
};
