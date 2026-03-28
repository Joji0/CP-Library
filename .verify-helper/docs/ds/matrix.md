---
title: Matrix
documentation_of: //ds/matrix.hpp
---

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
