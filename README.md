# CP-Library

Competitive programming library in C++17.

## Main Modules

- `monoid/`, `acted_monoid/`: algebraic building blocks for segment trees and lazy propagation
- `ds/`: Fenwick tree, segment trees, sqrt decomposition, treap, DSU, sparse table, PBDS wrappers, hashmap
- `graph/`: shortest paths, SCC, lowlink, MST, flows, matching, LCA, HLD, tree utilities
- `string/`: Z algorithm, prefix function, rolling hash, Manacher, trie, Aho-Corasick, suffix array
- `nt/`: sieves, CRT, modular helpers, Miller-Rabin, Pollard Rho, floor sum
- `convolution/`: NTT convolution
- `geometry/`: 2D point/vector primitives, intersections, convex hull, rotating calipers
- `dp/`: LIS, Li Chao tree, divide-and-conquer DP helper, Knuth optimization helper
- `linalg/`: Gaussian elimination, determinant/rank, XOR basis
- `mod/`: static and dynamic modint, Barrett reduction, combinatorics
- `docs/`: human-written documentation used by oj-verify

## Verification

Tests are under `test/`. Run `oj-verify run` in an environment with GNU C++17 support for PBDS-dependent headers.
