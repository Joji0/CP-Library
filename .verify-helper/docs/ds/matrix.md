---
title: Matrix
documentation_of: //ds/matrix.hpp
---

Matrix library with matrix exponentiation.

### Usage

- `Matrix<T>(int n, int m)`: $n \times m$ zero matrix
- `Matrix<T>::identity(int n)`: $n \times n$ identity matrix
- `A + B`, `A - B`, `A * B`: matrix arithmetic
- `A.pow(k)`: $A^k$ via repeated squaring
