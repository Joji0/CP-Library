#pragma once
#include <cassert>
#include <cstdint>
#include <vector>

template <typename T> struct Matrix {
        int H, W;
        std::vector<std::vector<T>> table;
        Matrix(int h, int w) : H(h), W(w), table(h, std::vector<T>(w)) {}
        Matrix(const std::vector<std::vector<T>> &v) : H((int)v.size()), W((int)v[0].size()), table(v) {}
        std::vector<T> &operator[](int i) { return table[i]; }
        const std::vector<T> &operator[](int i) const { return table[i]; }
        static Matrix identity(int N) {
                Matrix res(N, N);
                for (int i = 0; i < N; i++) {
                        res[i][i] = 1;
                }
                return res;
        }
        Matrix &operator+=(const Matrix &rhs) {
                assert(H == rhs.H && W == rhs.W && "DIMENSION must be the same");
                for (int i = 0; i < H; i++) {
                        for (int j = 0; j < W; j++) {
                                table[i][j] += rhs[i][j];
                        }
                }
                return *this;
        }
        Matrix &operator-=(const Matrix &rhs) {
                assert(H == rhs.H && W == rhs.W && "DIMENSION must be the same");
                for (int i = 0; i < H; i++) {
                        for (int j = 0; j < W; j++) {
                                table[i][j] -= rhs[i][j];
                        }
                }
                return *this;
        }
        Matrix operator*(const Matrix &rhs) const {
                assert(W == rhs.H && "MULTIPLICATION DIMENSION does not match");
                Matrix res(H, rhs.W);
                for (int i = 0; i < H; i++) {
                        for (int j = 0; j < W; j++) {
                                if (table[i][j] == 0) continue;
                                for (int k = 0; k < rhs.W; k++) {
                                        res[i][k] += table[i][j] * rhs[j][k];
                                }
                        }
                }
                return res;
        }
        Matrix &operator*=(const Matrix &rhs) { return *this = *this * rhs; }
        Matrix pow(int64_t n) const {
                assert(H == W && "DIMENSION must be square");
                Matrix res = identity(H);
                Matrix a = *this;
                while (n > 0) {
                        if (n & 1) res *= a;
                        a *= a;
                        n >>= 1;
                }
                return res;
        }
};
