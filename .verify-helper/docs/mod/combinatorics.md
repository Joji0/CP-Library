---
title: Combinatorics
documentation_of: mod/combinatorics.hpp
---

Combinatorics over ModInt. Lazy precomputation, $O(1)$ per query.

### Usage

- `C<Mint>(n, r)`: $\binom{n}{r}$
- `P<Mint>(n, r)`: ${}_{n}P_{r}$
- `H<Mint>(n, r)`: $\binom{n+r-1}{r}$ (stars and bars)
- `fact<Mint>(n)`: $n!$
- `inv<Mint>(n)`: $n^{-1}$
- `invfact<Mint>(n)`: $(n!)^{-1}$
- `catalan<Mint>(n)`: Catalan number $C_n$
- `derangement<Mint>(n)`: derangement count $D_n$
