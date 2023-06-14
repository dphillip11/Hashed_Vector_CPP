#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

template <typename dataType>
class hashedVector{
private:
	// nodes contain data about available keys
	struct node
	{
		int value;
		bool deleted = false;
	};

	// this is where user data is stored
	std::vector<dataType> wrapped_vector;
	// this maps keys to vector positions
	std::unordered_map<int, node> hash_map;
	// keeps key state
	int next_key = 0;

	void offsetAllValuesAfterKey(int key)
	{
		//offset all values after key
		for (auto &pair : hash_map)
		{
			if (pair.first > key)
			{
				pair.second.value--;
			}
		}
	}

	void add_node()
	{
		//add new node to hash map
		node new_node;
		//keep index of latest element in vector
		new_node.value = wrapped_vector.size() - 1;
		//add new node to hash map
		hash_map[next_key] = new_node;
		next_key++;
	}

public:
	int push_back(const dataType& new_data)
	{
		wrapped_vector.push_back(new_data);
		//add node to hash map
		add_node();
		return next_key-1;
	}

	//pass constructor arguments
	template <typename... Args>
	int emplace_back(Args&&... args)
	{
		wrapped_vector.emplace_back(std::forward<Args>(args)...);
		//add node to hash map
		add_node();
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
		wrapped_vector.erase(wrapped_vector.begin() + hash_map[key].value);
		//remove from hash map
		hash_map.erase(key);
		offsetAllValuesAfterKey(key);
	}

	auto& getVector()
	{
		return wrapped_vector;
	}

	auto getHashMap()
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
		return wrapped_vector[hash_map[key].value];
	}

	bool is_valid_key(const int& key)
	{
		return hash_map.find(key) != hash_map.end() && !hash_map[key].deleted;
	}

	//empties the data structure, ID's do not reset as this could allow false lookup for old keys
	//create new data structure if you want to reset ID's
	void clear()
	{
		wrapped_vector.clear();
		hash_map.clear();
	}
};

