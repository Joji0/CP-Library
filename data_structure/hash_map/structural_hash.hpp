#pragma once
#include <array>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace structural_hash_detail {
inline std::uint64_t mix(std::uint64_t value) noexcept {
	value += 0x9e3779b97f4a7c15ULL;
	value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
	value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;
	return value ^ (value >> 31);
}

template <typename>
inline constexpr bool unsupported_key = false;
}  // namespace structural_hash_detail

// A custom key may supply hash_fields() const, returning a tuple of the fields used by equality.
struct StructuralHash {
       private:
	static std::uint64_t append(std::uint64_t state, std::uint64_t field) noexcept {
		return structural_hash_detail::mix(state ^ structural_hash_detail::mix(field));
	}

	template <typename Range>
	std::uint64_t ordered(const Range &values) const {
		std::uint64_t state = 0, count = 0;
		for (const auto &value : values) {
			state = append(state, (*this)(value));
			++count;
		}
		return append(state, count);
	}

	template <typename Range>
	std::uint64_t unordered(const Range &values) const {
		std::uint64_t sum = 0, parity = 0;
		for (const auto &value : values) {
			const auto hash = structural_hash_detail::mix((*this)(value));
			sum += hash;
			parity ^= hash;
		}
		return append(append(sum, parity), values.size());
	}

       public:
	template <typename T>
	std::uint64_t operator()(const T &value) const {
		if constexpr (requires { value.hash_fields(); }) {
			return (*this)(value.hash_fields());
		} else if constexpr (std::is_same_v<T, __int128> || std::is_same_v<T, unsigned __int128>) {
			const auto bits = static_cast<unsigned __int128>(value);
			return append(std::uint64_t(bits), std::uint64_t(bits >> 64));
		} else if constexpr (requires { std::hash<T>{}(value); }) {
			return std::uint64_t(std::hash<T>{}(value));
		} else if constexpr (requires { std::tuple_size<T>::value; }) {
			return std::apply([&](const auto &...fields) {
				std::uint64_t state = 0;
				((state = append(state, (*this)(fields))), ...);
				return append(state, sizeof...(fields));
			},
					  value);
		} else if constexpr (requires { static_cast<std::uint64_t>(value); }) {
			return static_cast<std::uint64_t>(value);
		} else {
			static_assert(structural_hash_detail::unsupported_key<T>,
				      "Provide hash_fields(), a std::hash specialization, or a custom HashMap hasher.");
		}
	}

	template <typename... Ts>
	std::uint64_t operator()(const std::vector<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::deque<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::list<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::forward_list<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::set<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::multiset<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::map<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::multimap<Ts...> &values) const { return ordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::unordered_set<Ts...> &values) const { return unordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::unordered_multiset<Ts...> &values) const { return unordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::unordered_map<Ts...> &values) const { return unordered(values); }
	template <typename... Ts>
	std::uint64_t operator()(const std::unordered_multimap<Ts...> &values) const { return unordered(values); }

	template <typename T>
	std::uint64_t operator()(const std::optional<T> &value) const {
		return value ? append(1, (*this)(*value)) : 0;
	}

	template <typename... Ts>
	std::uint64_t operator()(const std::variant<Ts...> &value) const {
		if (value.valueless_by_exception()) return 0;
		return append(value.index() + 1, std::visit([&](const auto &part) { return (*this)(part); }, value));
	}
};
