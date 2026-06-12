---
title: NTT
documentation_of: //convolution/ntt.hpp
---

Number theoretic transform convolution over a prime modint.

### Usage

- `ntt<Mint>(a, invert)`: in-place transform
- `convolution<Mint>(a, b)`: polynomial convolution

Works with `ModInt<998244353>` and other NTT-friendly prime moduli with primitive roots.
