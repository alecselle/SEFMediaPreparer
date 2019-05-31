#ifndef SEFLIB_MAP_HPP
#define SEFLIB_MAP_HPP
#pragma once

#include <SuperEpicFuntime/Pair.hpp>

#include <initializer_list>
#include <vector>
using std::initializer_list, std::vector;

namespace SuperEpicFuntime::SEFLib {

/** ================================================================================================
 * (Class) Map
 */
template <class K, class V> class Map {
  private:
	vector<Pair<K, V>> mapPairs;

  public:
	Map() {
	}

	Map(Pair<K, V> pair) {
		add(pair);
	}

	Map(vector<Pair<K, V>> pairs) {
		addAll(pairs);
	}

	Map(initializer_list<Pair<K, V>> pairs) {
		vector<Pair<K, V>> p = {pairs};
		addAll(p);
	}

	void add(Pair<K, V> pair) {
		if (!exists(pair)) {
			mapPairs.push_back(pair);
		}
	}

	void add(K key, V value) {
		Pair<K, V> p = Pair<K, V>(key, value);
		add(p);
	}

	void addAll(vector<Pair<K, V>> pairs) {
		for (auto p : pairs) {
			add(p);
		}
	}

	void remove(K key) {
		int i = find(key);
		if (i >= 0) {
			mapPairs.erase(mapPairs.begin() + i);
		}
	}

	void remove(Pair<K, V> pair) {
		remove(pair.key());
	}

	V at(int i) {
		if (i < mapPairs.size()) {
			return mapPairs[i].value();
		}
	}

	Pair<K, V> pairAt(int i) {
		if (i < mapPairs.size()) {
			return mapPairs[i];
		}
	}

	V get(K key) {
		int i = find(key);
		if (i >= 0) {
			return mapPairs[i].value();
		}
	}

	Pair<K, V> getPair(K key) {
		int i = find(key);
		if (i >= 0) {
			return mapPairs[i];
		}
	}

	int find(K key) {
		for (int i = 0; i < mapPairs.size(); i++) {
			Pair<K, V> p = pairAt(i);
			if (p.key() == key) {
				return i;
			}
		}
		return -1;
	}

	bool exists(K key) {
		return (find(key) >= 0);
	}

	bool exists(Pair<K, V> pair) {
		return exists(pair.key());
	}

	int size() {
		return mapPairs.size();
	}

	void clear() {
		mapPairs.clear();
	}

	vector<Pair<K, V>> getPairs() {
		return mapPairs;
	}

	vector<K> getKeys() {
		vector<K> keys;
		for (auto p : mapPairs) {
			keys.push_back(p.key());
		}
		return keys;
	}

	vector<V> getValues() {
		vector<V> values;
		for (auto p : mapPairs) {
			values.push_back(p.value());
		}
		return values;
	}

	Map<V, K> reverse() {
		Map<V, K> r;
		for (auto p : mapPairs) {
			r.add(p.reverse());
		}
		return r;
	}

	V operator[](K key) {
		return get(key);
	}

	void operator+(Pair<K, V> pair) {
		add(pair);
	}

	void operator-(K key) {
		remove(key);
	}

	void operator-(Pair<K, V> pair) {
		remove(pair);
	}
};

} // namespace SuperEpicFuntime::SEFLib
#endif // SEFLIB_MAP_HPP
