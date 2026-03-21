---
layout: default
title: Home
---

# CP-Library

A competitive programming library in C++. Header-only, well-tested, and documented in ACL style.

<div class="category-grid">

<a class="category-card" href="{{ '/docs/ds/segtree/segtree' | relative_url }}">
  <div class="card-icon">🌲</div>
  <h3>Segment Trees</h3>
  <p>SegTree, LazySegTree, PersistentSegTree — point/range update, range query, binary search on tree</p>
  <div class="card-count">3 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/unionfind/unionfind' | relative_url }}">
  <div class="card-icon">🔗</div>
  <h3>Union-Find</h3>
  <p>DSU with path compression + union by size, rollback variant for offline algorithms</p>
  <div class="card-count">2 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/treap/implicit_treap' | relative_url }}">
  <div class="card-icon">🎲</div>
  <h3>Implicit Treap</h3>
  <p>Sequence with insert/erase, range update/query/reverse — the Swiss army knife of data structures</p>
  <div class="card-count">1 template</div>
</a>

<a class="category-card" href="{{ '/docs/ds/ordered_set' | relative_url }}">
  <div class="card-icon">📊</div>
  <h3>Order Statistics</h3>
  <p>OrderedSet, OrderedMultiSet — k-th element, order of key, range counting in O(log n)</p>
  <div class="card-count">2 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/hashmap' | relative_url }}">
  <div class="card-icon">⚡</div>
  <h3>Other DS</h3>
  <p>HashMap (anti-hash), SparseTable (O(1) RMQ), Matrix (exponentiation)</p>
  <div class="card-count">3 templates</div>
</a>

<a class="category-card" href="{{ '/docs/monoid/add' | relative_url }}">
  <div class="card-icon">🧮</div>
  <h3>Monoid</h3>
  <p>Plug-and-play monoids: Add, Max, Min, Xor, Gcd, Mul — drop into SegTree directly</p>
  <div class="card-count">6 templates</div>
</a>

<a class="category-card" href="{{ '/docs/acted_monoid/sum_add' | relative_url }}">
  <div class="card-icon">🎯</div>
  <h3>ActedMonoid</h3>
  <p>Range Add/Assign/Affine × Sum/Max/Min — all 9 combinations for LazySegTree</p>
  <div class="card-count">9 templates</div>
</a>

<a class="category-card" href="{{ '/docs/mod/modint' | relative_url }}">
  <div class="card-icon">🔢</div>
  <h3>Modular Arithmetic</h3>
  <p>ModInt, DynamicModInt, Barrett reduction, Combinatorics (C/P/H, Catalan, derangements)</p>
  <div class="card-count">4 templates</div>
</a>

<a class="category-card" href="{{ '/docs/nt/sieve' | relative_url }}">
  <div class="card-icon">🔍</div>
  <h3>Number Theory</h3>
  <p>Linear sieve: primes, SPF, Euler's totient, Möbius, factorization, divisors</p>
  <div class="card-count">1 template</div>
</a>

<a class="category-card" href="{{ '/docs/bigdec/bigdec' | relative_url }}">
  <div class="card-icon">📐</div>
  <h3>BigDec</h3>
  <p>Fixed-point decimal with __int128_t — exact arithmetic without floating-point errors</p>
  <div class="card-count">1 template</div>
</a>

</div>
