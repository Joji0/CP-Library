---
title: BigDec
documentation_of: bigdec/bigdec.hpp
---

# BigDec (Fixed-Point Decimal)

## Overview

A **fixed-point decimal** arithmetic type using `__int128_t` as the underlying storage. This allows exact decimal arithmetic with $P$ decimal places of precision, without floating-point rounding errors.

Useful in competitive programming for problems requiring:

- Exact decimal comparison (e.g., "is $a/b > c/d$?")
- Fixed-precision arithmetic where `double` is insufficient
- Reading decimal input without precision loss

The internal representation stores $\mathrm{val} = \mathrm{value} \times 10^P$ as a 128-bit integer, so the range is approximately $\pm 1.7 \times 10^{38} / 10^P$.

## Template Parameter

| Parameter | Description |
|-----------|-------------|
| `P` | Number of decimal places (precision). For example, `BigDec<9>` stores 9 decimal digits after the point. |

## Constructors

### `BigDec<P>()`

Creates a BigDec with value $0$.

### `BigDec<P>(int64_t n)`

Creates a BigDec with integer value $n$ (i.e., $n.000\ldots$).

**Complexity**: $O(P)$

### `BigDec<P>(std::string s)`

Creates a BigDec by parsing a decimal string like `"3.14159"`. The string may include a negative sign, integer part, and fractional part. Digits beyond $P$ decimal places are truncated.

**Complexity**: $O(|s|)$

## Operators

Full arithmetic and comparison operators:

| Operator | Description | Complexity |
|----------|-------------|------------|
| `a + b`, `a - b` | Addition / subtraction | $O(1)$ |
| `a * b` | Multiplication (result truncated to $P$ decimals) | $O(1)$ |
| `a / b` | Division (result truncated to $P$ decimals) | $O(1)$ |
| `+a`, `-a` | Unary plus / negation | $O(1)$ |
| `a < b`, `a > b`, `a <= b`, `a >= b` | Comparison | $O(1)$ |
| `a == b`, `a != b` | Equality | $O(1)$ |
| `cin >> a` | Read decimal string from input | $O(|s|)$ |
| `cout << a` | Output with exactly $P$ decimal digits | $O(P)$ |

All compound assignment versions (`+=`, `-=`, `*=`, `/=`) are also available.

## Members

| Member | Description |
|--------|-------------|
| `val` | The internal `__int128_t` representation: actual value $\times 10^P$ |

## Precision Notes

- Multiplication: $(a \times b)$ internally computes a 256-bit product $a.\mathrm{val} \times b.\mathrm{val}$ and divides by $10^P$, which may lose the lowest bits.
- Division: similarly, the result is $a.\mathrm{val} \times 10^P / b.\mathrm{val}$.
- For very large values (close to `__int128_t` limits), multiplication and division may overflow.
- Safe range (approximate): values up to $\sim 10^{38-P}$.

## Usage Example

```cpp
#include "bigdec/bigdec.hpp"

int main() {
    using Dec = BigDec<9>; // 9 decimal places

    Dec a("3.141592653");
    Dec b("2.718281828");

    Dec c = a + b;  // 5.859874481
    Dec d = a * b;  // 8.539734222 (truncated to 9 decimals)
    Dec e = a / b;  // 1.155727349

    if (a > b) { /* true */ }

    std::cin >> a;   // reads a decimal number
    std::cout << a;  // prints with exactly 9 decimal places
}
```
