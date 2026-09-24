#pragma once
#include <vector>
#include <list>
#include <optional>
#include <ranges>
#include "Bucket.hpp"

//HashMap["alma"] = 10;
// alma -> hash -> 102 -> index = 102 % capacity -> HashMap[index] = Bucket

template<typename Key, typename Value>
class HashMap
{
public:
	HashMap(size_t init_capacity = 16);

	Value& operator[](Key key);

	void insert(Key key, Value value);
	void remove(Key key);
	bool contains(Key key);
	const size_t size() const;
	const bool empty() const;
	const std::pair<Key, Value>* begin() const;
	const std::pair<Key, Value>* end() const;
	void clear();

private:
	size_t getIndexByKey(Key key, size_t size) const;
	void resize();

	size_t elementCount;
	std::vector<Bucket<Key, Value>> bucket;
};

template<typename Key, typename Value>
size_t HashMap<typename Key, typename Value>::getIndexByKey(Key key, size_t size) const
{
	return std::hash<Key>{}(key) % size;
}

template<typename Key, typename Value>
HashMap<typename Key, typename Value>::HashMap(size_t init_capacity)
{
	this->bucket = {};
	this->bucket.resize(init_capacity);

	elementCount = 0;
}


template<typename Key, typename Value>
Value& HashMap<typename Key, typename Value>::operator[](Key key)
{
	bool increaseElementCount = false;
	resize();
	size_t index = getIndexByKey(key, bucket.size());
	
	auto& val = this->bucket[index].GetValueByKey(key, increaseElementCount);
	if (increaseElementCount)
		this->elementCount++;

	return val;
}

template<typename Key, typename Value>
void HashMap<typename Key, typename Value>::insert(Key key, Value value) 
{
	bool increaseElementCount = false;
	resize();
	size_t index = getIndexByKey(key, bucket.size());
	this->bucket[index].Insert(key, value, increaseElementCount);

	if (increaseElementCount) {
		this->elementCount++;
	}
		
}

template<typename Key, typename Value>
bool HashMap<typename Key, typename Value>::contains(Key key)
{
	size_t index = getIndexByKey(key, bucket.size());
	std::pair<Key, Value>* pair = this->bucket[index].FindPair(key);
	if (pair != nullptr)
		return true;

	return false;
}

template<typename Key, typename Value>
void HashMap<typename Key, typename Value>::resize() {
	if (bucket.size() > elementCount)
		return;

	size_t oldSize = this->bucket.size();
	std::vector<Bucket<Key, Value>> newBucket(oldSize * 2);
	bool temp;

	for (size_t i{ 0 }; i < oldSize; i++) {
		for (auto& j : this->bucket[i].bucket) {
			size_t index = getIndexByKey(j.first, oldSize * 2);
			newBucket[index].Insert(j.first, j.second, temp);
		}
	}

	this->bucket = newBucket;
}

template<typename Key, typename Value>
void HashMap<typename Key, typename Value>::remove(Key key) {
	size_t index = getIndexByKey(key, bucket.size());
	bool decreaseElementCount = false;
	this->bucket[index].Remove(key, decreaseElementCount);

	if (this->elementCount)
		this->elementCount--;

}

template<typename Key, typename Value>
const size_t HashMap<typename Key, typename Value>::size() const{
	return this->elementCount;
}

template<typename Key, typename Value>
const bool HashMap<typename Key, typename Value>::empty() const{
	return this->elementCount == 0;
}

template<typename Key, typename Value>
const std::pair<Key, Value>* HashMap<typename Key, typename Value>::begin() const {
	for (size_t i{ 0 }; i < this->bucket.size(); i++) {
		for (auto& j : this->bucket[i].bucket) {
			if (j.first)
				return &j;
		}
	}
}

template<typename Key, typename Value>
const std::pair<Key, Value>* HashMap<typename Key, typename Value>::end() const{
	for (size_t i{ this->bucket.size() - 1 }; i > 0; i--) {
		for (auto& j : this->bucket[i].bucket | std::views::reverse) {
			if (j.first)
				return &j;
		}
	}
}

template<typename Key, typename Value>
void HashMap<typename Key, typename Value>::clear() {
	for (size_t i{ 0 }; i < this->bucket.size(); i++) {
		this->bucket[i].bucket.clear();
	}

	this->elementCount = 0;
}