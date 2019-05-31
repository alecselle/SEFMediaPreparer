#ifndef SEFLIB_PAIR_HPP
#define SEFLIB_PAIR_HPP
#pragma once

#include <vector>
using std::vector;

namespace SuperEpicFuntime::SEFLib {

/** ================================================================================================
 * (Class) Pair
 */
template <class K, class V> class Pair {
  private:
	K pairKey;
	V pairValue;

  public:
	Pair(K key, V value) {
		pairKey   = key;
		pairValue = value;
	}

	K key() {
		return pairKey;
	}

	V value() {
		return pairValue;
	}

	V operator()() {
		return pairValue;
	}

	void operator=(V newValue) {
		pairValue = newValue;
	}

	Pair<V, K> reverse() {
		return Pair<V, K>(pairValue, pairKey);
	}

	static vector<Pair<K, V>> Pairs(vector<K> keys, vector<V> values) {
		vector<Pair<K, V>> pairs;
		for (int i = 0; i < keys.size() && i < values.size(); i++) {
			pairs.push_back(Pair(keys[i], values[i]));
		}
		return pairs;
	}
};

} // namespace SuperEpicFuntime::SEFLib
#endif // SEFLIB_PAIR_HPP
