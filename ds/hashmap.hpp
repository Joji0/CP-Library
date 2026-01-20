#pragma once
#include <cassert>
#include <chrono>
#include <cstdint>
#include <vector>

template <typename K, typename V> struct HashMap {
      private:
        struct Node {
                K key;
                V val;
                bool exists = false;
        };
        std::vector<Node> table;
        uint32_t cap, size;
        uint64_t random_seed;
        static uint64_t splitmix64(uint64_t x) {
                // http://xorshift.di.unimi.it/splitmix64.c
                x += 0x9e3779b97f4a7c15;
                x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
                x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
                return x ^ (x >> 31);
        }
        uint32_t get_index(K key) {
                uint64_t hash = splitmix64((uint64_t)key + random_seed);
                return hash & (cap - 1);
        }
        void expand() {
                std::vector<Node> old_table = table;
                build(cap << 1);
                for (const auto &node : old_table) {
                        if (node.exists) {
                                uint32_t idx = get_index(node.key);
                                while (table[idx].exists) {
                                        idx++;
                                        idx &= (cap - 1);
                                }
                                table[idx] = {node.key, node.val, true};
                                size++;
                        }
                }
        }

      public:
        HashMap(uint32_t n = 1 << 20) {
                uint32_t pw = 1;
                while (pw < n) pw <<= 1;
                build(pw);
        }
        void build(uint32_t n) {
                cap = n;
                size = 0;
                table.assign(cap, {});
                random_seed = std::chrono::steady_clock::now().time_since_epoch().count();
        }
        void insert(K key, V val) { (*this)[key] = val; }
        V &operator[](K key) {
                if (size >= cap / 2) expand();
                uint32_t idx = get_index(key);
                while (table[idx].exists) {
                        if (table[idx].key == key) {
                                return table[idx].val;
                        }
                        idx++;
                        idx &= (cap - 1);
                }
                table[idx] = {key, V{}, true};
                size++;
                return table[idx].val;
        }
        V get(K key, V default_val = -1) {
                uint32_t idx = get_index(key);
                while (table[idx].exists) {
                        if (table[idx].key == key) {
                                return table[idx].val;
                        }
                        idx++;
                        idx &= (cap - 1);
                }
                return default_val;
        }
        int sz() { return size; }
};
