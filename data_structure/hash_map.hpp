#pragma once
#include <algorithm>
#include <bit>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include "data_structure/hash_map/structural_hash.hpp"

// Open-addressed hash map with stable mapped-value addresses across insertion and rehash.
// Equivalent keys must produce the same hash.
template <typename K, typename V, typename Hasher = StructuralHash, typename KeyEqual = std::equal_to<K>>
class HashMap {
	using Index = std::uint32_t;
	static constexpr Index empty_bucket = 0;
	static constexpr Index deleted_bucket = std::numeric_limits<Index>::max();
	static constexpr std::size_t maximum_capacity = std::size_t(1) << 31;

	struct Entry {
		std::uint64_t hash;
		K key;
		V value;

		template <typename Key, typename... Args>
		Entry(std::uint64_t hash, Key &&key, Args &&...args)
		    : hash(hash), key(std::forward<Key>(key)), value(std::forward<Args>(args)...) {}
	};
	struct Slot {
		std::optional<Entry> entry;
		Index next_free = 0;
	};
	struct Probe {
		std::size_t bucket;
		bool found;
	};

	struct State {
		[[no_unique_address]] Hasher hasher;
		[[no_unique_address]] KeyEqual equal;
		std::uint64_t seed;
		std::vector<Index> buckets;
		std::deque<Slot> slots;
		Index free_slot = 0;
		std::size_t count = 0, deleted = 0;

		State(std::size_t capacity, Hasher hasher, KeyEqual equal)
		    : hasher(std::move(hasher)), equal(std::move(equal)), seed(std::chrono::steady_clock::now().time_since_epoch().count()), buckets(capacity) {}

		std::uint64_t hash_key(const K &key) const {
			return structural_hash_detail::mix(std::uint64_t(hasher(key)) + seed);
		}

		Probe probe(const K &key, std::uint64_t hash) const {
			const std::size_t mask = buckets.size() - 1;
			std::size_t bucket = std::size_t(hash) & mask;
			std::size_t first_deleted = buckets.size();
			while (buckets[bucket] != empty_bucket) {
				const Index id = buckets[bucket];
				if (id == deleted_bucket) {
					if (first_deleted == buckets.size()) first_deleted = bucket;
				} else {
					const auto &entry = *slots[id - 1].entry;
					if (entry.hash == hash && equal(entry.key, key)) return {bucket, true};
				}
				bucket = (bucket + 1) & mask;
			}
			return {first_deleted == buckets.size() ? bucket : first_deleted, false};
		}

		static std::size_t empty_slot(const std::vector<Index> &table, std::uint64_t hash) {
			const std::size_t mask = table.size() - 1;
			std::size_t bucket = std::size_t(hash) & mask;
			while (table[bucket] != empty_bucket) bucket = (bucket + 1) & mask;
			return bucket;
		}

		void rehash(std::size_t capacity) {
			std::vector<Index> next(capacity);
			for (Index id : buckets) {
				if (id == empty_bucket || id == deleted_bucket) continue;
				const auto hash = slots[id - 1].entry->hash;
				next[empty_slot(next, hash)] = id;
			}
			buckets.swap(next);
			deleted = 0;
		}

		template <typename Key, typename... Args>
		std::pair<V *, bool> emplace(Key &&key, Args &&...args) {
			const auto hash = hash_key(key);
			auto position = probe(key, hash);
			if (position.found) return {std::addressof(slots[buckets[position.bucket] - 1].entry->value), false};
			if (buckets[position.bucket] == empty_bucket && count + deleted >= buckets.size() / 2) {
				if (count >= buckets.size() / 2) {
					if (buckets.size() == maximum_capacity) throw std::length_error("HashMap capacity exceeded");
					rehash(buckets.size() * 2);
				} else {
					rehash(buckets.size());
				}
				position.bucket = empty_slot(buckets, hash);
			}

			Index id = free_slot;
			if (id != 0) {
				auto &slot = slots[id - 1];
				slot.entry.emplace(hash, std::forward<Key>(key), std::forward<Args>(args)...);
				free_slot = slot.next_free;
			} else {
				slots.emplace_back();
				try {
					slots.back().entry.emplace(hash, std::forward<Key>(key), std::forward<Args>(args)...);
				} catch (...) {
					slots.pop_back();
					throw;
				}
				id = Index(slots.size());
			}
			if (buckets[position.bucket] == deleted_bucket) --deleted;
			buckets[position.bucket] = id;
			++count;
			return {std::addressof(slots[id - 1].entry->value), true};
		}
	};

	std::unique_ptr<State> state;
	static constexpr bool copyable = std::is_copy_constructible_v<K> && std::is_copy_constructible_v<V> &&
					 std::is_copy_constructible_v<Hasher> && std::is_copy_constructible_v<KeyEqual>;

	static std::size_t capacity_for_slots(std::size_t slots) {
		if (slots > maximum_capacity) throw std::length_error("HashMap capacity exceeded");
		return std::bit_ceil(std::max(slots, std::size_t(2)));
	}

	State &writable_state() {
		if (!state) {
			if constexpr (std::is_default_constructible_v<Hasher> && std::is_default_constructible_v<KeyEqual>) {
				state = std::make_unique<State>(2, Hasher{}, KeyEqual{});
			} else {
				throw std::logic_error("Assign a new HashMap before mutating this moved-from object");
			}
		}
		return *state;
	}

	template <typename Key, typename Mapped>
	std::pair<V *, bool> assign(Key &&key, Mapped &&value) {
		auto [entry, inserted] = writable_state().emplace(std::forward<Key>(key), std::forward<Mapped>(value));
		if (!inserted) *entry = std::forward<Mapped>(value);
		return {entry, inserted};
	}

       public:
	HashMap(std::size_t slots = 16, Hasher hasher = Hasher{}, KeyEqual equal = KeyEqual{})
	    : state(std::make_unique<State>(capacity_for_slots(slots), std::move(hasher), std::move(equal))) {}

	HashMap(const HashMap &other)
		requires(copyable)
	    : state(other.state ? std::make_unique<State>(*other.state) : nullptr) {}
	HashMap &operator=(const HashMap &other)
		requires(copyable)
	{
		if (this != &other) state = other.state ? std::make_unique<State>(*other.state) : nullptr;
		return *this;
	}
	HashMap(HashMap &&) noexcept = default;
	HashMap &operator=(HashMap &&) noexcept = default;

	int size() const { return state ? int(state->count) : 0; }
	bool empty() const { return size() == 0; }
	std::size_t bucket_count() const { return state ? state->buckets.size() : 0; }

	// Resets the table with at least this many slots.
	void build(std::size_t slots) {
		auto &current = writable_state();
		std::vector<Index> next(capacity_for_slots(slots));
		clear();
		current.buckets.swap(next);
	}

	// Reserves room for this many keys without growth.
	void reserve(std::size_t keys) {
		if (keys > maximum_capacity / 2) throw std::length_error("HashMap capacity exceeded");
		auto &current = writable_state();
		const std::size_t needed = capacity_for_slots(keys * 2);
		if (needed > current.buckets.size() || current.deleted != 0) {
			current.rehash(std::max(needed, current.buckets.size()));
		}
	}

	void clear() {
		if (!state) return;
		std::fill(state->buckets.begin(), state->buckets.end(), empty_bucket);
		state->slots.clear();
		state->free_slot = 0;
		state->count = 0;
		state->deleted = 0;
	}

	// A returned pointer remains valid until its key is erased, the map is reset, or the map is destroyed.
	const V *find(const K &key) const {
		if (!state) return nullptr;
		const auto position = state->probe(key, state->hash_key(key));
		return position.found ? std::addressof(state->slots[state->buckets[position.bucket] - 1].entry->value) : nullptr;
	}
	V *find(const K &key) { return const_cast<V *>(std::as_const(*this).find(key)); }
	bool contains(const K &key) const { return find(key) != nullptr; }
	V get(const K &key, V default_value = V{}) const {
		const V *value = find(key);
		if (value) return *value;
		return std::move_if_noexcept(default_value);
	}

	template <typename... Args>
	std::pair<V *, bool> try_emplace(const K &key, Args &&...args) {
		return writable_state().emplace(key, std::forward<Args>(args)...);
	}
	template <typename... Args>
	std::pair<V *, bool> try_emplace(K &&key, Args &&...args) {
		return writable_state().emplace(std::move(key), std::forward<Args>(args)...);
	}

	template <typename Mapped>
	std::pair<V *, bool> insert_or_assign(const K &key, Mapped &&value) {
		return assign(key, std::forward<Mapped>(value));
	}
	template <typename Mapped>
	std::pair<V *, bool> insert_or_assign(K &&key, Mapped &&value) {
		return assign(std::move(key), std::forward<Mapped>(value));
	}

	// Compatibility with the original insert API, which overwrites an existing value.
	void insert(K key, V value) {
		insert_or_assign(std::move_if_noexcept(key), std::move_if_noexcept(value));
	}

	V &operator[](const K &key)
		requires std::is_default_constructible_v<V>
	{ return *try_emplace(key).first; }
	V &operator[](K &&key)
		requires std::is_default_constructible_v<V>
	{ return *try_emplace(std::move(key)).first; }

	bool erase(const K &key) {
		if (!state) return false;
		const auto position = state->probe(key, state->hash_key(key));
		if (!position.found) return false;
		const Index id = state->buckets[position.bucket];
		auto &slot = state->slots[id - 1];
		slot.entry.reset();
		slot.next_free = state->free_slot;
		state->free_slot = id;
		state->buckets[position.bucket] = deleted_bucket;
		--state->count;
		++state->deleted;
		return true;
	}
};
