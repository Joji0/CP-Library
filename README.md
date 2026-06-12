# CP-Library

Competitive programming library in C++17.

This README tracks the committed library surface. Experimental or learning-in-progress files in the worktree are intentionally omitted until they are learned, verified, and committed.

## Committed Modules

- `monoid/`, `acted_monoid/`: algebraic building blocks for segment trees and lazy propagation
- `mod/`: static and dynamic modint, Barrett reduction, combinatorics
- `ds/`: Fenwick tree, segment trees, treap, DSU, sparse table, matrix, PBDS wrappers, hashmap
- `graph/`: LCA
- `nt/`: linear sieve
- `convolution/`: NTT convolution
- `bigdec/`: fixed-precision decimal arithmetic
- `docs/`: human-written documentation used by oj-verify

## Verification

Tests are under `test/`. Run `oj-verify run` in an environment with GNU C++17 support for PBDS-dependent headers.
