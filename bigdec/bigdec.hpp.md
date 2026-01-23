---
data:
  _extendedDependsOn: []
  _extendedRequiredBy: []
  _extendedVerifiedWith:
  - icon: ':heavy_check_mark:'
    path: test/2_yukicoder/81.test.cpp
    title: test/2_yukicoder/81.test.cpp
  _isVerificationFailed: false
  _pathExtension: hpp
  _verificationStatusIcon: ':heavy_check_mark:'
  attributes:
    links: []
  bundledCode: "#line 2 \"bigdec/bigdec.hpp\"\n#include <algorithm>\n#include <cstdint>\n\
    #include <iostream>\n#include <string>\n\ntemplate <int P> struct BigDec {\n \
    \       using i128 = __int128_t;\n        i128 val;\n        static i128 get_scale()\
    \ {\n                i128 x = 1;\n                for (int i = 0; i < P; i++)\
    \ x *= 10;\n                return x;\n        }\n        BigDec() : val(0) {}\n\
    \        BigDec(int64_t n) : val((i128)n * get_scale()) {}\n        BigDec(std::string\
    \ s) { read_string(s); }\n        void read_string(std::string s) {\n        \
    \        val = 0;\n                if (s.empty()) return;\n                bool\
    \ negative = (s[0] == '-');\n                if (negative) s = s.substr(1);\n\
    \                size_t dot = s.find('.');\n                std::string s_int\
    \ = (dot == std::string::npos) ? s : s.substr(0, dot);\n                std::string\
    \ s_frac = (dot == std::string::npos) ? \"\" : s.substr(dot + 1);\n          \
    \      i128 int_part = 0;\n                if (!s_int.empty()) {\n           \
    \             for (char c : s_int) {\n                                int_part\
    \ = int_part * 10 + (c - '0');\n                        }\n                }\n\
    \                i128 frac_part = 0;\n                if (s_frac.length() > P)\
    \ s_frac = s_frac.substr(0, P);\n                while (s_frac.length() < P) s_frac\
    \ += \"0\";\n                for (char c : s_frac) frac_part = frac_part * 10\
    \ + (c - '0');\n                val = int_part * get_scale() + frac_part;\n  \
    \              if (negative) val = -val;\n        }\n        BigDec operator+(const\
    \ BigDec &other) const {\n                BigDec ret;\n                ret.val\
    \ = val + other.val;\n                return ret;\n        }\n        BigDec &operator+=(const\
    \ BigDec &other) {\n                val += other.val;\n                return\
    \ *this;\n        }\n        BigDec operator-(const BigDec &other) const {\n \
    \               BigDec ret;\n                ret.val = val - other.val;\n    \
    \            return ret;\n        }\n        BigDec &operator-=(const BigDec &other)\
    \ {\n                val -= other.val;\n                return *this;\n      \
    \  }\n        BigDec operator+() const {\n                BigDec ret;\n      \
    \          ret.val = val;\n                return ret;\n        }\n        BigDec\
    \ operator-() const {\n                BigDec ret;\n                ret.val =\
    \ -val;\n                return ret;\n        }\n        BigDec operator*(const\
    \ BigDec &other) const {\n                BigDec ret;\n                ret.val\
    \ = (val * other.val) / get_scale();\n                return ret;\n        }\n\
    \        BigDec &operator*=(const BigDec &other) {\n                val = (val\
    \ * other.val) / get_scale();\n                return *this;\n        }\n    \
    \    BigDec operator/(const BigDec &other) const {\n                BigDec ret;\n\
    \                ret.val = (val * get_scale()) / other.val;\n                return\
    \ ret;\n        }\n        BigDec &operator/=(const BigDec &other) {\n       \
    \         val = (val * get_scale()) / other.val;\n                return *this;\n\
    \        }\n        bool operator<(const BigDec &other) const { return val < other.val;\
    \ }\n        bool operator>(const BigDec &other) const { return val > other.val;\
    \ }\n        bool operator<=(const BigDec &other) const { return val <= other.val;\
    \ }\n        bool operator>=(const BigDec &other) const { return val >= other.val;\
    \ }\n        bool operator==(const BigDec &other) const { return val == other.val;\
    \ }\n        bool operator!=(const BigDec &other) const { return val != other.val;\
    \ }\n        friend std::istream &operator>>(std::istream &is, BigDec &bd) {\n\
    \                std::string s;\n                is >> s;\n                bd.read_string(s);\n\
    \                return is;\n        }\n        friend std::ostream &operator<<(std::ostream\
    \ &os, const BigDec &bd) {\n                i128 temp = bd.val;\n            \
    \    if (temp < 0) {\n                        os << \"-\";\n                 \
    \       temp = -temp;\n                }\n                i128 scale = get_scale();\n\
    \                i128 int_part = temp / scale;\n                i128 frac_part\
    \ = temp % scale;\n                std::string s_int = \"\";\n               \
    \ if (int_part == 0)\n                        s_int = \"0\";\n               \
    \ else {\n                        i128 t = int_part;\n                       \
    \ while (t > 0) {\n                                s_int += (char)('0' + (t %\
    \ 10));\n                                t /= 10;\n                        }\n\
    \                        std::reverse(s_int.begin(), s_int.end());\n         \
    \       }\n                os << s_int;\n                os << \".\";\n      \
    \          std::string s_frac = \"\";\n                i128 t_frac = frac_part;\n\
    \                for (int i = 0; i < P; ++i) {\n                        s_frac\
    \ += (char)('0' + (t_frac % 10));\n                        t_frac /= 10;\n   \
    \             }\n                std::reverse(s_frac.begin(), s_frac.end());\n\
    \                os << s_frac;\n                return os;\n        }\n};\n"
  code: "#pragma once\n#include <algorithm>\n#include <cstdint>\n#include <iostream>\n\
    #include <string>\n\ntemplate <int P> struct BigDec {\n        using i128 = __int128_t;\n\
    \        i128 val;\n        static i128 get_scale() {\n                i128 x\
    \ = 1;\n                for (int i = 0; i < P; i++) x *= 10;\n               \
    \ return x;\n        }\n        BigDec() : val(0) {}\n        BigDec(int64_t n)\
    \ : val((i128)n * get_scale()) {}\n        BigDec(std::string s) { read_string(s);\
    \ }\n        void read_string(std::string s) {\n                val = 0;\n   \
    \             if (s.empty()) return;\n                bool negative = (s[0] ==\
    \ '-');\n                if (negative) s = s.substr(1);\n                size_t\
    \ dot = s.find('.');\n                std::string s_int = (dot == std::string::npos)\
    \ ? s : s.substr(0, dot);\n                std::string s_frac = (dot == std::string::npos)\
    \ ? \"\" : s.substr(dot + 1);\n                i128 int_part = 0;\n          \
    \      if (!s_int.empty()) {\n                        for (char c : s_int) {\n\
    \                                int_part = int_part * 10 + (c - '0');\n     \
    \                   }\n                }\n                i128 frac_part = 0;\n\
    \                if (s_frac.length() > P) s_frac = s_frac.substr(0, P);\n    \
    \            while (s_frac.length() < P) s_frac += \"0\";\n                for\
    \ (char c : s_frac) frac_part = frac_part * 10 + (c - '0');\n                val\
    \ = int_part * get_scale() + frac_part;\n                if (negative) val = -val;\n\
    \        }\n        BigDec operator+(const BigDec &other) const {\n          \
    \      BigDec ret;\n                ret.val = val + other.val;\n             \
    \   return ret;\n        }\n        BigDec &operator+=(const BigDec &other) {\n\
    \                val += other.val;\n                return *this;\n        }\n\
    \        BigDec operator-(const BigDec &other) const {\n                BigDec\
    \ ret;\n                ret.val = val - other.val;\n                return ret;\n\
    \        }\n        BigDec &operator-=(const BigDec &other) {\n              \
    \  val -= other.val;\n                return *this;\n        }\n        BigDec\
    \ operator+() const {\n                BigDec ret;\n                ret.val =\
    \ val;\n                return ret;\n        }\n        BigDec operator-() const\
    \ {\n                BigDec ret;\n                ret.val = -val;\n          \
    \      return ret;\n        }\n        BigDec operator*(const BigDec &other) const\
    \ {\n                BigDec ret;\n                ret.val = (val * other.val)\
    \ / get_scale();\n                return ret;\n        }\n        BigDec &operator*=(const\
    \ BigDec &other) {\n                val = (val * other.val) / get_scale();\n \
    \               return *this;\n        }\n        BigDec operator/(const BigDec\
    \ &other) const {\n                BigDec ret;\n                ret.val = (val\
    \ * get_scale()) / other.val;\n                return ret;\n        }\n      \
    \  BigDec &operator/=(const BigDec &other) {\n                val = (val * get_scale())\
    \ / other.val;\n                return *this;\n        }\n        bool operator<(const\
    \ BigDec &other) const { return val < other.val; }\n        bool operator>(const\
    \ BigDec &other) const { return val > other.val; }\n        bool operator<=(const\
    \ BigDec &other) const { return val <= other.val; }\n        bool operator>=(const\
    \ BigDec &other) const { return val >= other.val; }\n        bool operator==(const\
    \ BigDec &other) const { return val == other.val; }\n        bool operator!=(const\
    \ BigDec &other) const { return val != other.val; }\n        friend std::istream\
    \ &operator>>(std::istream &is, BigDec &bd) {\n                std::string s;\n\
    \                is >> s;\n                bd.read_string(s);\n              \
    \  return is;\n        }\n        friend std::ostream &operator<<(std::ostream\
    \ &os, const BigDec &bd) {\n                i128 temp = bd.val;\n            \
    \    if (temp < 0) {\n                        os << \"-\";\n                 \
    \       temp = -temp;\n                }\n                i128 scale = get_scale();\n\
    \                i128 int_part = temp / scale;\n                i128 frac_part\
    \ = temp % scale;\n                std::string s_int = \"\";\n               \
    \ if (int_part == 0)\n                        s_int = \"0\";\n               \
    \ else {\n                        i128 t = int_part;\n                       \
    \ while (t > 0) {\n                                s_int += (char)('0' + (t %\
    \ 10));\n                                t /= 10;\n                        }\n\
    \                        std::reverse(s_int.begin(), s_int.end());\n         \
    \       }\n                os << s_int;\n                os << \".\";\n      \
    \          std::string s_frac = \"\";\n                i128 t_frac = frac_part;\n\
    \                for (int i = 0; i < P; ++i) {\n                        s_frac\
    \ += (char)('0' + (t_frac % 10));\n                        t_frac /= 10;\n   \
    \             }\n                std::reverse(s_frac.begin(), s_frac.end());\n\
    \                os << s_frac;\n                return os;\n        }\n};\n"
  dependsOn: []
  isVerificationFile: false
  path: bigdec/bigdec.hpp
  requiredBy: []
  timestamp: '2026-01-21 22:01:49+07:00'
  verificationStatus: LIBRARY_ALL_AC
  verifiedWith:
  - test/2_yukicoder/81.test.cpp
documentation_of: bigdec/bigdec.hpp
layout: document
redirect_from:
- /library/bigdec/bigdec.hpp
- /library/bigdec/bigdec.hpp.html
title: bigdec/bigdec.hpp
---
