#pragma once
#include <vector>
#include <list>
#include <optional>

//HashMap["alma"] = 10;
// alma -> hash -> 102 -> index = 102 % capacity -> HashMap[index] = Bucket

template<typename Key, typename Value>
struct Bucket
{
	std::list<std::pair<Key, Value>> bucket;

	Bucket(Key key, Value value) {
		bucket = { {key, value} };
	}

	Bucket() {
		bucket = {};
	}

	std::pair<Key, Value>* FindPair(Key key) {
		for (auto& i : bucket) {
			if (i.first == key) {
				return &i;
			}
		}

		return nullptr;
	}

	Value& GetValueByKey(Key key, bool& increaseElementCount) {

		std::pair<Key, Value>* pairptr = FindPair(key);

		if (pairptr != nullptr)
			return (*pairptr).second;

		bucket.push_back({ key, Value{} });
		increaseElementCount = true;
		return bucket.back().second;
	}

	void Insert(Key key, Value value, bool& increaseElementCount) {
		std::pair<Key, Value>* pairptr = FindPair(key);

		if (pairptr != nullptr) {
			(*pairptr).first = key;
			(*pairptr).second = value;
			return;
		}

		bucket.push_back({ key, value });
		increaseElementCount = true;
	}

	void Remove(Key key, bool& decreaseElementCount) {
		std::pair<Key, Value>* pairptr = FindPair(key);
		if (pairptr == nullptr)
			return;

		decreaseElementCount = true;
		bucket.remove(std::pair<Key, Value>{(*pairptr).first, (*pairptr).second});
	}
};