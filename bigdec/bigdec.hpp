#pragma once
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>

template <int P> struct BigDec {
        using i128 = __int128_t;
        i128 val;
        static i128 get_scale() {
                i128 x = 1;
                for (int i = 0; i < P; i++) x *= 10;
                return x;
        }
        BigDec() : val(0) {}
        BigDec(int64_t n) : val((i128)n * get_scale()) {}
        BigDec(std::string s) { read_string(s); }
        void read_string(std::string s) {
                val = 0;
                if (s.empty()) return;
                bool negative = (s[0] == '-');
                if (negative) s = s.substr(1);
                size_t dot = s.find('.');
                std::string s_int = (dot == std::string::npos) ? s : s.substr(0, dot);
                std::string s_frac = (dot == std::string::npos) ? "" : s.substr(dot + 1);
                i128 int_part = 0;
                if (!s_int.empty()) {
                        for (char c : s_int) {
                                int_part = int_part * 10 + (c - '0');
                        }
                }
                i128 frac_part = 0;
                if (s_frac.length() > P) s_frac = s_frac.substr(0, P);
                while (s_frac.length() < P) s_frac += "0";
                for (char c : s_frac) frac_part = frac_part * 10 + (c - '0');
                val = int_part * get_scale() + frac_part;
                if (negative) val = -val;
        }
        BigDec operator+(const BigDec &other) const {
                BigDec ret;
                ret.val = val + other.val;
                return ret;
        }
        BigDec &operator+=(const BigDec &other) {
                val += other.val;
                return *this;
        }
        BigDec operator-(const BigDec &other) const {
                BigDec ret;
                ret.val = val - other.val;
                return ret;
        }
        BigDec &operator-=(const BigDec &other) {
                val -= other.val;
                return *this;
        }
        BigDec operator+() const {
                BigDec ret;
                ret.val = val;
                return ret;
        }
        BigDec operator-() const {
                BigDec ret;
                ret.val = -val;
                return ret;
        }
        BigDec operator*(const BigDec &other) const {
                BigDec ret;
                ret.val = (val * other.val) / get_scale();
                return ret;
        }
        BigDec &operator*=(const BigDec &other) {
                val = (val * other.val) / get_scale();
                return *this;
        }
        BigDec operator/(const BigDec &other) const {
                BigDec ret;
                ret.val = (val * get_scale()) / other.val;
                return ret;
        }
        BigDec &operator/=(const BigDec &other) {
                val = (val * get_scale()) / other.val;
                return *this;
        }
        bool operator<(const BigDec &other) const { return val < other.val; }
        bool operator>(const BigDec &other) const { return val > other.val; }
        bool operator<=(const BigDec &other) const { return val <= other.val; }
        bool operator>=(const BigDec &other) const { return val >= other.val; }
        bool operator==(const BigDec &other) const { return val == other.val; }
        bool operator!=(const BigDec &other) const { return val != other.val; }
        friend std::istream &operator>>(std::istream &is, BigDec &bd) {
                std::string s;
                is >> s;
                bd.read_string(s);
                return is;
        }
        friend std::ostream &operator<<(std::ostream &os, const BigDec &bd) {
                i128 temp = bd.val;
                if (temp < 0) {
                        os << "-";
                        temp = -temp;
                }
                i128 scale = get_scale();
                i128 int_part = temp / scale;
                i128 frac_part = temp % scale;
                std::string s_int = "";
                if (int_part == 0)
                        s_int = "0";
                else {
                        i128 t = int_part;
                        while (t > 0) {
                                s_int += (char)('0' + (t % 10));
                                t /= 10;
                        }
                        std::reverse(s_int.begin(), s_int.end());
                }
                os << s_int;
                os << ".";
                std::string s_frac = "";
                i128 t_frac = frac_part;
                for (int i = 0; i < P; ++i) {
                        s_frac += (char)('0' + (t_frac % 10));
                        t_frac /= 10;
                }
                std::reverse(s_frac.begin(), s_frac.end());
                os << s_frac;
                return os;
        }
};
