---
title: ModInt
documentation_of: //mod/modint.hpp
---

## Overview

A **compile-time** modular integer type. All arithmetic operations automatically reduce modulo $M$, eliminating manual `% MOD` calls and preventing overflow bugs.

Supports:

- Full arithmetic: $+$, $-$, $\times$, $\div$
- Modular exponentiation in $O(\log n)$
- Modular inverse via extended Euclidean algorithm in $O(\log M)$
- Stream I/O (`cin >> x`, `cout << x`)
- `constexpr` evaluation where possible

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `MOD` | A `uint32_t` constant $> 1$. Must be prime for division/inverse to work correctly. |

### Common Instantiations

```cpp
using Mint = ModInt<998244353>;
using Mint = ModInt<1000000007>;
```

## Constructor

### `ModInt()`

Creates a ModInt with value $0$.

### `ModInt(int64_t x)`

Creates a ModInt with value $x \bmod M$. Handles negative values correctly: $\mathrm{ModInt}(-1)$ yields $M - 1$.

**Complexity**: $O(1)$

## Operators

All arithmetic operators are provided and follow standard semantics modulo $M$:

| Operator | Description |
|----------|-------------|
| `a + b` | Addition $\pmod{M}$ |
| `a - b` | Subtraction $\pmod{M}$ |
| `a * b` | Multiplication $\pmod{M}$ |
| `a / b` | Division $\pmod{M}$ (requires $M$ prime) |
| `a += b`, `a -= b`, `a *= b`, `a /= b` | Compound assignment |
| `+a` | Unary plus (identity) |
| `-a` | Unary negation: returns $M - a$ |
| `a == b`, `a != b` | Equality comparison |
| `cin >> a`, `cout << a` | Stream I/O |

**Complexity**: $O(1)$ for all except `/` which is $O(\log M)$

## Methods

### `uint32_t value()`

Returns the stored value in $[0, M)$.

**Complexity**: $O(1)$

### `static uint32_t mod()`

Returns $M$.

**Complexity**: $O(1)$

### `ModInt pow(uint64_t n)`

Returns $\mathrm{this}^n \bmod M$ using binary exponentiation.

**Constraints**: $n \ge 0$

**Complexity**: $O(\log n)$

### `ModInt inverse()`

Returns $\mathrm{this}^{-1} \bmod M$ using the extended Euclidean algorithm.

**Constraints**: $\gcd(\mathrm{val}, M) = 1$ (always true if $M$ is prime and $\mathrm{val} \ne 0$)

**Complexity**: $O(\log M)$

## Usage Example

```cpp
#include "mod/modint.hpp"

using Mint = ModInt<998244353>;

int main() {
    Mint a = 3, b = 5;

    Mint c = a + b;     // 8
    Mint d = a * b;     // 15
    Mint e = a - b;     // 998244351

    Mint inv3 = a.inverse();    // 3^{-1} mod 998244353
    Mint pow3 = a.pow(100);     // 3^{100} mod 998244353

    Mint f = Mint(10) / Mint(3); // 10 * 3^{-1} mod 998244353
}
```
