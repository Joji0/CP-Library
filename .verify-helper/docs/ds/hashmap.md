---
title: HashMap
documentation_of: //ds/hashmap.hpp
---

## Overview

A custom **open-addressing hash map** optimized for competitive programming. Compared to `std::unordered_map`, this provides:

- Faster constant factors due to open addressing with linear probing
- Anti-hack protection via **randomized hashing** (splitmix64 with a random seed based on clock)
- Automatic resizing when load factor exceeds 50%

Designed for integer keys. Supports $O(1)$ amortized insert and lookup.

## Template Parameters

| Parameter | Description |
|-----------|-------------|
| `K` | Key type. Must be castable to `uint64_t`. Typically `int` or `int64_t`. |
| `V` | Value type. Must be default-constructible. |

## Constructor

### `HashMap<K, V>(uint32_t n = 1 << 20)`

Creates a hash map with initial capacity rounded up to the next power of 2 from $n$. Default capacity is $2^{20} \approx 10^6$.

The capacity auto-doubles when occupancy exceeds 50%.

**Complexity**: $O(n)$

## Methods

### `void insert(K key, V val)`

Inserts the key-value pair $(key, val)$. If `key` already exists, its value is overwritten. Equivalent to `map[key] = val`.

**Complexity**: $O(1)$ amortized

### `V &operator[](K key)`

Returns a reference to the value associated with `key`. If `key` does not exist, it is inserted with value `V{}` (default-constructed, typically $0$).

**Complexity**: $O(1)$ amortized

### `V get(K key, V default_val = -1)`

Returns the value associated with `key`, or `default_val` if `key` is not present.

**Complexity**: $O(1)$ amortized

### `int sz()`

Returns the number of key-value pairs stored.

**Complexity**: $O(1)$

### `void build(uint32_t n)`

Resets the hash map with a new capacity $n$ and a fresh random seed.

**Complexity**: $O(n)$

## Implementation Details

- **Hash function**: [splitmix64](https://xorshift.di.unimi.it/splitmix64.c) applied to `key + random_seed`
- **Collision resolution**: Linear probing
- **Load factor threshold**: 50% (triggers 2x expansion)
- **Random seed**: Derived from `std::chrono::steady_clock` at construction time

## Usage Example

```cpp
#include "ds/hashmap.hpp"

int main() {
    HashMap<int64_t, int> mp;

    mp[1000000007] = 42;
    mp.insert(998244353, 7);

    mp.get(1000000007);      // 42
    mp.get(123, -1);         // -1 (not found)
    mp[998244353];           // 7
    mp.sz();                 // 2
}
```
