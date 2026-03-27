---
title: Matrix
documentation_of: ds/matrix.hpp
---

# Matrix

## Overview

A generic matrix type supporting standard arithmetic operations and fast exponentiation. Useful for:

- **Matrix exponentiation** for linear recurrences in $O(d^3 \log k)$
- **Matrix multiplication** for combinatorial problems
- **System transformations** in competitive programming

where $d$ is the matrix dimension and $k$ is the exponent.

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `T` | Element type. Must support `+`, `-`, `*`, `+=`, and default-construct to $0$. Commonly `int64_t` or `ModInt<MOD>`. |

## Constructors

### `Matrix<T>(int h, int w)`

Creates an $h \times w$ matrix with all entries initialized to the default value of `T` (typically $0$).

**Constraints**: $h, w \ge 0$

**Complexity**: $O(hw)$

### `Matrix<T>(const std::vector<std::vector<T>> &v)`

Creates a matrix from a 2D vector $v$.

**Constraints**: $v$ must be non-empty and all rows must have equal length.

**Complexity**: $O(hw)$

## Static Methods

### `Matrix<T> Matrix<T>::identity(int N)`

Returns the $N \times N$ identity matrix $I_N$.

**Complexity**: $O(N^2)$

## Operators

### `std::vector<T> &operator[](int i)`

Returns a reference to row $i$. Allows `mat[i][j]` access.

**Constraints**: $0 \le i < H$

### `Matrix operator+(Matrix rhs)` / `Matrix operator-(Matrix rhs)`

Element-wise addition / subtraction.

**Constraints**: Both matrices must have the same dimensions.

**Complexity**: $O(HW)$

### `Matrix operator*(const Matrix &rhs)`

Standard matrix multiplication.

**Constraints**: Number of columns of `*this` must equal number of rows of `rhs`.

**Complexity**: $O(H \cdot W \cdot W')$ where $W' = \mathrm{rhs.W}$

The implementation skips zero entries for a constant-factor speedup.

## Methods

### `Matrix pow(int64_t n)`

Returns `*this` raised to the $n$-th power using binary exponentiation.

**Constraints**:
- The matrix must be square ($H = W$)
- $n \ge 0$

**Complexity**: $O(d^3 \log n)$ where $d = H = W$

## Members

| Member | Description |
|--------|-------------|
| `H` | Number of rows |
| `W` | Number of columns |
| `table` | The underlying `std::vector<std::vector<T>>` |

## Usage Example

```cpp
#include "ds/matrix.hpp"
#include "mod/modint.hpp"

using Mint = ModInt<998244353>;

int main() {
    // Fibonacci via matrix exponentiation
    // [F(n+1)] = [1 1]^n * [1]
    // [F(n)  ]   [1 0]     [0]
    Matrix<Mint> A({{1, 1}, {1, 0}});
    auto An = A.pow(50);
    Mint fib50 = An[1][0]; // F(50)
}
```

## Verified with

* **AC** [yukicoder No.3227](https://yukicoder.me/problems/no/3227) &mdash; `3227.test.cpp`
* **AC** [yukicoder No.3228](https://yukicoder.me/problems/no/3228) &mdash; `3228.test.cpp`

## Source Code

```cpp
#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

template <typename T> struct Matrix {
        int H, W;
        std::vector<std::vector<T>> table;
        Matrix(int h, int w) : H(h), W(w), table(h, std::vector<T>(w)) {}
        Matrix(const std::vector<std::vector<T>> &v) : H((int)v.size()), W((int)v[0].size()), table(v) {}
        std::vector<T> &operator[](int i) { return table[i]; }
        const std::vector<T> &operator[](int i) const { return table[i]; }
        static Matrix identity(int N) {
                Matrix res(N, N);
                for (int i = 0; i < N; i++) {
                        res[i][i] = 1;
                }
                return res;
        }
        Matrix &operator+=(const Matrix &rhs) {
                assert(H == rhs.H && W == rhs.W && "DIMENSION must be the same");
                for (int i = 0; i < H; i++) {
                        for (int j = 0; j < W; j++) {
                                table[i][j] += rhs[i][j];
                        }
                }
                return *this;
        }
        Matrix &operator-=(const Matrix &rhs) {
                assert(H == rhs.H && W == rhs.W && "DIMENSION must be the same");
                for (int i = 0; i < H; i++) {
                        for (int j = 0; j < W; j++) {
                                table[i][j] -= rhs[i][j];
                        }
                }
                return *this;
        }
        Matrix operator*(const Matrix &rhs) const {
                assert(W == rhs.H && "MULTIPLICATION DIMENSION does not match");
                Matrix res(H, rhs.W);
                for (int i = 0; i < H; i++) {
                        for (int j = 0; j < W; j++) {
                                if (table[i][j] == 0) continue;
                                for (int k = 0; k < rhs.W; k++) {
                                        res[i][k] += table[i][j] * rhs[j][k];
                                }
                        }
                }
                return res;
        }
        Matrix &operator*=(const Matrix &rhs) { return *this = *this * rhs; }
        Matrix pow(int64_t n) const {
                assert(H == W && "DIMENSION must be square");
                Matrix res = identity(H);
                Matrix a = *this;
                while (n > 0) {
                        if (n & 1) res *= a;
                        a *= a;
                        n >>= 1;
                }
                return res;
        }
};
```
