#pragma once
#include <cstdint>
#include <utility>

// reference: https://maspypy.github.io/library/mod/barrett.hpp
struct Barrett {
        uint32_t mod;
        uint64_t inv_mod;
        explicit Barrett(uint32_t m = 1) : mod(m), inv_mod((uint64_t)(-1) / m + 1) {}
        uint32_t umod() const { return mod; }
        uint32_t modulo(uint64_t z) const {
                if (mod == 1) return 0;
                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);
                uint64_t y = x * mod;
                return (uint32_t)(z - y + (z < y ? mod : 0));
        }
        uint64_t floor(uint64_t z) const {
                if (mod == 1) return z;
                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);
                uint64_t y = x * mod;
                return (z < y ? x - 1 : x);
        }
        std::pair<uint64_t, uint32_t> divmod(uint64_t z) const {
                if (mod == 1) return {z, 0};
                uint64_t x = (uint64_t)(((__uint128_t)(z)*inv_mod) >> 64);
                uint64_t y = x * mod;
                if (z < y) return {x - 1, z - y + mod};
                return {x, z - y};
        }
        uint32_t mul(uint32_t a, uint32_t b) { return modulo((uint64_t)(a)*b); }
};
