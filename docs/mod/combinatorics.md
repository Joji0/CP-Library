---
title: Combinatorics
documentation_of: mod/combinatorics.hpp
---

# Combinatorics

## Overview

A comprehensive combinatorics utility struct providing precomputed factorials, inverse factorials, and a rich set of combinatorial functions — all under modular arithmetic.

Key features:

- **Lazy precomputation**: Tables are extended on demand with amortized doubling, so you never precompute more than needed.
- **$O(n)$ precomputation, $O(1)$ per query** for all standard functions.
- **Large $n$ support**: $\binom{n}{k}$ works even for $n$ up to $10^{18}$ (with $O(k)$ fallback).

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `Mint` | A modular integer type (e.g., `ModInt<998244353>`). Must provide `Mint::mod()`, arithmetic operators, and `inverse()`. |

### Common Instantiation

```cpp
using Mint = ModInt<998244353>;
using C = Combinatorics<Mint>;
```

## Precomputed Tables

All tables are **static** and shared across instances. They grow lazily — calling any function with argument $n$ ensures tables are extended to at least size $n$.

| Table | Description |
|-------|-------------|
| `fact[n]` | $n! \bmod M$ |
| `inv[n]` | $n^{-1} \bmod M$ |
| `invfact[n]` | $(n!)^{-1} \bmod M$ |
| `der[n]` | $D_n$ — the number of [derangements](https://en.wikipedia.org/wiki/Derangement) of $n$ elements |

## Static Methods

### `void ensure(int n)`

Ensures all tables are computed up to index $n$. Called automatically by other methods.

**Complexity**: $O(n)$ on first call; amortized $O(1)$ for subsequent calls with smaller $n$.

### `Mint fact(int n)`

Returns $n! \bmod M$. Returns $0$ if $n < 0$.

**Complexity**: $O(1)$ after precomputation

### `Mint inv(int n)`

Returns $n^{-1} \bmod M$. For $n < 0$, computes the inverse directly.

**Complexity**: $O(1)$ after precomputation

### `Mint invfact(int n)`

Returns $(n!)^{-1} \bmod M$. Returns $0$ if $n < 0$.

**Complexity**: $O(1)$ after precomputation

### `Mint C(int64_t n, int64_t k)`

Returns $\binom{n}{k} \bmod M$. Returns $0$ if $k < 0$ or $k > n$.

For $n < 2 \times 10^7$: uses precomputed factorials in $O(1)$.

For $n \ge 2 \times 10^7$: falls back to $O(k)$ direct computation.

**Complexity**: $O(1)$ or $O(k)$ depending on $n$

### `Mint P(int64_t n, int64_t k)`

Returns $P(n, k) = \frac{n!}{(n-k)!} \bmod M$ — the number of $k$-permutations of $n$.

Returns $0$ if $k < 0$ or $k > n$.

**Complexity**: $O(1)$ or $O(k)$ depending on $n$

### `Mint H(int64_t n, int64_t k)`

Returns $H(n, k) = \binom{n+k-1}{k} \bmod M$ — the number of multisets of size $k$ from $n$ types. Also known as "combinations with repetition" or "stars and bars".

**Complexity**: Same as `C`

### `Mint stars_and_bars(int64_t n, int64_t k, bool allow_empty = true)`

Distributes $n$ identical objects into $k$ distinct bins.

- `allow_empty = true`: Each bin may be empty. Returns $\binom{n+k-1}{k-1}$.
- `allow_empty = false`: Each bin must have at least one. Returns $\binom{n-1}{k-1}$.

**Complexity**: Same as `C`

### `Mint catalan(int n)`

Returns the $n$-th [Catalan number](https://en.wikipedia.org/wiki/Catalan_number):

$$C_n = \frac{1}{n+1}\binom{2n}{n}$$

**Complexity**: $O(1)$ after precomputation

### `Mint derangement(int n)`

Returns $D_n$, the number of [derangements](https://en.wikipedia.org/wiki/Derangement) (permutations with no fixed points) of $n$ elements. Computed via the recurrence:

$$D_n = (n-1)(D_{n-1} + D_{n-2})$$

**Complexity**: $O(1)$ after precomputation

### `Mint multinomial(int n, const std::vector<int> &ks)`

Returns the [multinomial coefficient](https://en.wikipedia.org/wiki/Multinomial_theorem):

$$\binom{n}{k_1, k_2, \ldots, k_m} = \frac{n!}{k_1! \cdot k_2! \cdot \ldots \cdot k_m!}$$

Returns $0$ if $\sum k_i \ne n$ or any $k_i < 0$.

**Complexity**: $O(m)$ after precomputation, where $m = |ks|$

## Usage Example

```cpp
#include "mod/modint.hpp"
#include "mod/combinatorics.hpp"

using Mint = ModInt<998244353>;
using C = Combinatorics<Mint>;

int main() {
    C::C(10, 3);           // C(10,3) = 120
    C::P(10, 3);           // P(10,3) = 720
    C::H(5, 3);            // H(5,3) = C(7,3) = 35
    C::catalan(5);         // 42
    C::derangement(5);     // 44
    C::fact(10);           // 10! mod 998244353
    C::inv(7);             // 7^{-1} mod 998244353

    C::multinomial(10, {3, 3, 4}); // 10!/(3!3!4!)

    // Large n
    C::C(1000000000000LL, 5); // C(10^12, 5) in O(5)

    // Stars and bars: distribute 10 items into 3 bins
    C::stars_and_bars(10, 3, true);  // bins can be empty
    C::stars_and_bars(10, 3, false); // bins must be non-empty
}
```
