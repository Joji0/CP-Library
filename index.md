---
layout: default
title: Home
render_with_liquid: true
---

<div class="hero">
  <h1>CP-Library</h1>
  <p class="hero-subtitle">
    Header-only C++ library for competitive programming.<br>Well-tested, documented in ACL style.
  </p>
</div>

<div class="category-grid">

<a class="category-card" href="{{ '/docs/ds/segtree/segtree' | relative_url }}">
  <div class="card-label">Data Structures</div>
  <h3>Segment Trees</h3>
  <p>SegTree, LazySegTree, PersistentSegTree &mdash; point/range update, range query, binary search on tree</p>
  <div class="card-count">3 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/unionfind/unionfind' | relative_url }}">
  <div class="card-label">Data Structures</div>
  <h3>Union-Find</h3>
  <p>DSU with path compression + union by size, rollback variant for offline algorithms</p>
  <div class="card-count">2 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/treap/implicit_treap' | relative_url }}">
  <div class="card-label">Data Structures</div>
  <h3>Implicit Treap</h3>
  <p>Sequence with insert/erase, range update/query/reverse &mdash; the Swiss army knife</p>
  <div class="card-count">1 template</div>
</a>

<a class="category-card" href="{{ '/docs/ds/ordered_set' | relative_url }}">
  <div class="card-label">Data Structures</div>
  <h3>Order Statistics</h3>
  <p>OrderedSet, OrderedMultiSet &mdash; k-th element, order of key, range counting</p>
  <div class="card-count">2 templates</div>
</a>

<a class="category-card" href="{{ '/docs/ds/hashmap' | relative_url }}">
  <div class="card-label">Data Structures</div>
  <h3>HashMap / SparseTable / Matrix</h3>
  <p>Anti-hash map, O(1) range min, matrix exponentiation</p>
  <div class="card-count">3 templates</div>
</a>

<a class="category-card" href="{{ '/docs/monoid/add' | relative_url }}">
  <div class="card-label">Algebra</div>
  <h3>Monoid</h3>
  <p>Plug-and-play monoids &mdash; Add, Max, Min, Xor, Gcd, Mul for SegTree</p>
  <div class="card-count">6 templates</div>
</a>

<a class="category-card" href="{{ '/docs/acted_monoid/sum_add' | relative_url }}">
  <div class="card-label">Algebra</div>
  <h3>ActedMonoid</h3>
  <p>Range Add/Assign/Affine &times; Sum/Max/Min &mdash; all 9 combinations for LazySegTree</p>
  <div class="card-count">9 templates</div>
</a>

<a class="category-card" href="{{ '/docs/mod/modint' | relative_url }}">
  <div class="card-label">Math</div>
  <h3>Modular Arithmetic</h3>
  <p>ModInt, DynamicModInt, Barrett reduction, Combinatorics</p>
  <div class="card-count">4 templates</div>
</a>

<a class="category-card" href="{{ '/docs/nt/sieve' | relative_url }}">
  <div class="card-label">Math</div>
  <h3>Number Theory</h3>
  <p>Linear sieve &mdash; primes, SPF, Euler totient, M&ouml;bius, factorization</p>
  <div class="card-count">1 template</div>
</a>

<a class="category-card" href="{{ '/docs/bigdec/bigdec' | relative_url }}">
  <div class="card-label">Math</div>
  <h3>BigDec</h3>
  <p>Fixed-point decimal with __int128_t &mdash; exact arithmetic, no floating-point</p>
  <div class="card-count">1 template</div>
</a>

</div>
