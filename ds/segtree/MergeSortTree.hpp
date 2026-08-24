#pragma once
#include <algorithm>
#include <vector>

template <typename T>
class MergeSortTree {
        int n;
        std::vector<std::vector<T>> t;
        void build(const std::vector<T>& A, int v, int tl, int tr) {
                if (tl == tr) {
                        t[v] = { A[tl] };
                } else {
                        int tm = (tl + tr) / 2;
                        build(A, v * 2, tl, tm);
                        build(A, v * 2 + 1, tm + 1, tr);
                        t[v].resize(t[v * 2].size() + t[v * 2 + 1].size());
                        std::merge(t[v * 2].begin(), t[v * 2].end(), t[v * 2 + 1].begin(), t[v * 2 + 1].end(), t[v].begin());
                }
        }
        int count_greater_equal(int v, int tl, int tr, int l, int r, const T& val) const {
                if (l > r) return 0;
                if (l == tl && r == tr) {
                        return t[v].end() - std::lower_bound(t[v].begin(), t[v].end(), val);
                }
                int tm = (tl + tr) / 2;
                return count_greater_equal(v * 2, tl, tm, l, std::min(r, tm), val) +
                        count_greater_equal(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, val);
        }
        int count_less_equal(int v, int tl, int tr, int l, int r, const T& val) const {
                if (l > r) return 0;
                if (l == tl && r == tr) {
                        return std::upper_bound(t[v].begin(), t[v].end(), val) - t[v].begin();
                }
                int tm = (tl + tr) / 2;
                return count_less_equal(v * 2, tl, tm, l, std::min(r, tm), val) +
                        count_less_equal(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, val);
        }
        int count_greater(int v, int tl, int tr, int l, int r, const T& val) const {
                if (l > r) return 0;
                if (l == tl && r == tr) {
                        return t[v].end() - std::upper_bound(t[v].begin(), t[v].end(), val);
                }
                int tm = (tl + tr) / 2;
                return count_greater(v * 2, tl, tm, l, std::min(r, tm), val) +
                        count_greater(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, val);
        }
        int count_less(int v, int tl, int tr, int l, int r, const T& val) const {
                if (l > r) return 0;
                if (l == tl && r == tr) {
                        return std::lower_bound(t[v].begin(), t[v].end(), val) - t[v].begin();
                }
                int tm = (tl + tr) / 2;
                return count_less(v * 2, tl, tm, l, std::min(r, tm), val) +
                        count_less(v * 2 + 1, tm + 1, tr, std::max(l, tm + 1), r, val);
        }
public:
        MergeSortTree() : n(0) {}
        MergeSortTree(int n) : n(n) { t.resize(4 * n); }
        MergeSortTree(const std::vector<T>& A) : n((int)A.size()) {
                t.resize(4 * n);
                build(A, 1, 0, n - 1);
        }
        int count_greater_equal(int l, int r, const T& val) const {
                if (l > r) return 0;
                return count_greater_equal(1, 0, n - 1, l, r, val);
        }
        int count_less_equal(int l, int r, const T& val) const {
                if (l > r) return 0;
                return count_less_equal(1, 0, n - 1, l, r, val);
        }
        int count_greater(int l, int r, const T& val) const {
                if (l > r) return 0;
                return count_greater(1, 0, n - 1, l, r, val);
        }
        int count_less(int l, int r, const T& val) const {
                if (l > r) return 0;
                return count_less(1, 0, n - 1, l, r, val);
        }
};
