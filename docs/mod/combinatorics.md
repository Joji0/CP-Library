---
title: Combinatorics
documentation_of: //mod/combinatorics.hpp
---

Combinatorics over ModInt. Lazy precomputation, $O(1)$ per query.

### Usage

- `Combinatorics<Mint>::C(n, r)`: $\binom{n}{r}$
- `Combinatorics<Mint>::P(n, r)`: ${}_{n}P_{r}$
- `Combinatorics<Mint>::H(n, r)`: $\binom{n+r-1}{r}$ (stars and bars)
- `Combinatorics<Mint>::fact(n)`: $n!$
- `Combinatorics<Mint>::inv(n)`: $n^{-1}$
- `Combinatorics<Mint>::invfact(n)`: $(n!)^{-1}$
- `Combinatorics<Mint>::catalan(n)`: Catalan number $C_n$
- `Combinatorics<Mint>::derangement(n)`: derangement count $D_n$
