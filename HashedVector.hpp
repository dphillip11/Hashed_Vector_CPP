#ifndef HASHEDVECTOR_HPP
#define HASHEDVECTOR_HPP
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

template <typename dataType>
class hashedVector{
private:
	// this is where user data is stored
	std::vector<dataType> wrapped_vector;
	// this maps keys to vector positions
	std::unordered_map<int, int> hash_map;
	// keeps key state
	int next_key = 0;

	void offsetAllValuesAfterKey(int key)
	{
		//offset all values after key
		for (auto &pair : hash_map)
		{
			if (pair.first > key)
			{
				pair.second--;
			}
		}
	}

	void addPair()
	{
		//add new pair to hash map
		hash_map[next_key] = wrapped_vector.size() - 1;
		next_key++;
	}

public:
	//pushes data to vector and returns key
	int push_back(const dataType& new_data)
	{
		wrapped_vector.push_back(new_data);
		//add node to hash map
		addPair();
		return next_key-1;
	}

	//pass constructor arguments
	template <typename... Args>
	int emplace_back(Args&&... args)
	{
		wrapped_vector.emplace_back(std::forward<Args>(args)...);
		//add pair to hash map
		addPair();
		return next_key - 1;
	}

	void remove(const int& key)
	{
		//check if key is valid
		if (!is_valid_key(key))
		{
			throw std::invalid_argument("Key is not valid");
		}
		//remove data from vector
		wrapped_vector.erase(wrapped_vector.begin() + hash_map[key]);
		//remove from hash map
		hash_map.erase(key);
		offsetAllValuesAfterKey(key);
	}

	auto& getVector()
	{
		return wrapped_vector;
	}

	auto& getHashMap()
	{
		return hash_map;
	}

	int size()
	{
		return wrapped_vector.size();
	}

	void reserve(const int& size)
	{
		wrapped_vector.reserve(size);
	}

	dataType& operator[](const int& key)
	{
		//check if key is valid
		if (!is_valid_key(key))
		{
			throw std::invalid_argument("Key is not valid");
		}
		return wrapped_vector[hash_map[key]];
	}

	//check key value pair exists
	bool is_valid_key(const int& key) const
	{
		return hash_map.find(key) != hash_map.end();
	}

	//returns pointer to data if key is valid, otherwise returns nullptr
	dataType* try_get(int key)
	{
		if (!is_valid_key(key))
		{
			return nullptr;
		}
		return &wrapped_vector[hash_map[key]];
	}

	//empties the data structure, ID's do not reset as this could allow false lookup for old keys
	//create new data structure if you want to reset ID's
	void clear()
	{
		wrapped_vector.clear();
		hash_map.clear();
	}
};

#endif // !HASHEDVECTOR_HPP
