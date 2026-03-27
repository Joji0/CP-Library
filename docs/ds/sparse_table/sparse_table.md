---
title: Sparse Table
documentation_of: ds/sparse_table/sparse_table.hpp
---

# Sparse Table

## Overview

A data structure for **static** range queries on an [idempotent](https://en.wikipedia.org/wiki/Idempotence) operation $f$, i.e., $f(x, x) = x$.

Given an array $a_0, a_1, \ldots, a_{n-1}$, precomputes all results of $f$ on power-of-two-length subarrays, enabling:

- **Range query**: Compute $f(a_l, a_{l+1}, \ldots, a_r)$ in $O(1)$
- **Preprocessing** in $O(n \log n)$ time and space

The default operation is `std::min`. Common applications include Range Minimum Query (RMQ), which is a building block for LCA, suffix array algorithms, and more.

**Note**: The array is **static** — no updates are supported after construction.

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `T` | Element type. Must support comparison for the default `std::min` operation. |

The combine function `f` is currently hardcoded as `std::min(a, b)`. To use a different idempotent operation, modify the `f` method.

## Constructor

### `SparseTable<T>()`

Creates an empty sparse table. Call `init()` to populate.

## Methods

### `void init(const std::vector<T> &A)`

Builds the sparse table from array $A$ of size $n$.

**Constraints**: $0 < \lvert A \rvert$

**Complexity**: $O(n \log n)$ time and space

### `T query(int L, int R)`

Returns $f(a_L, a_{L+1}, \ldots, a_R) = \min(a_L, \ldots, a_R)$ with the default operation.

**Constraints**: $0 \le L \le R < n$

**Complexity**: $O(1)$

## Usage Example

```cpp
#include "ds/sparse_table/sparse_table.hpp"

int main() {
    std::vector<int> a = {5, 2, 4, 1, 3};
    SparseTable<int> st;
    st.init(a);

    st.query(0, 4); // min(5,2,4,1,3) = 1
    st.query(0, 1); // min(5,2) = 2
    st.query(2, 4); // min(4,1,3) = 1
}
```

## Source Code

```cpp
#pragma once

#include <algorithm>
#include <vector>
template <class T> struct SparseTable {
        int N = 0, LOG = 25;
        std::vector<std::vector<T>> st;
        T f(const T &a, const T &b) const { return std::min(a, b); }
        void init(const std::vector<T> &A) {
                N = (int)A.size();
                st.assign(LOG, vector<T>(N));
                for (int j = 0; j < N; j++) st[0][j] = A[j];
                for (int i = 1; i < LOG; i++) {
                        for (int j = 0; j + (1LL << i) - 1 < N; j++) {
                                st[i][j] = f(st[i - 1][j], st[i - 1][j + (1LL << (i - 1))]);
                        }
                }
        }
        T query(int L, int R) const {
                int len = R - L + 1;
                int lg = (int)log2(len);
                return f(st[lg][L], st[lg][R - (1 << lg) + 1]);
        }
};
```
