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
		int next;
		int prev;
		int value;
		bool deleted = false;
	};

	// this is where user data is stored
	std::vector<dataType> wrapped_vector;
	// this maps keys to vector positions
	std::unordered_map<int, node> hash_map;
	// keeps key state
	int next_key = 0;
	int first_key = -1;
	int last_key = -1;

	//remove key from hash_map and returns an offset to be applied to all keys
	int remove_key(int key)
	{
		int offset = 0;
		auto &node = hash_map[key];
		//mend the linked list
		if(node.next != -1)
			hash_map[node.next].prev = node.prev;
		if(node.prev != -1)
			hash_map[node.prev].next = node.next;
		//update first and last keys
		if(key == first_key)
		{
			offset = node.next - first_key;
			first_key = node.next;
		}
		if(key == last_key)
			last_key = node.prev;
		//remove from hash_map
		hash_map.erase(key);
	}

	void offsetAllValues(int offset)
	{
		for (auto& node : hash_map)
		{
			node.second.value -= offset;
		}
	}

	void increment_key()
	{
		auto key = next_key;
		//increment key for next push_back
		next_key++;
		// check if first key is unassigned
		if (first_key == -1)
		{
			first_key = key;
		}
		//if last key is assigned, update its next value
		if (last_key != -1)
		{
			hash_map[last_key].next = key;
		}
	}

	void add_node()
	{
		//add new node to hash map
		node new_node;
		//keep index of latest element in vector
		new_node.value = wrapped_vector.size() - 1;
		//keep track of previous key
		new_node.prev = last_key;
		//keep track of last key
		last_key = next_key - 1;
		//add new node to hash map
		hash_map[last_key] = new_node;
	}

public:
	int push_back(const dataType& new_data)
	{
		increment_key();
		// add new data to vector
		wrapped_vector.push_back(new_data);
		//add node to hash map
		add_node();
		return next_key-1;
	}

	//pass constructor arguments
	template <typename... Args>
	int emplace_back(Args&&... args)
	{
		increment_key();
		// add new data to vector
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
		//remove key from hash map
		int offset = remove_key(key);
		//offset all keys
		if (offset != 0)
			offsetAllValues(offset);
	}

	auto& getVector()
	{
		return wrapped_vector;
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
		first_key = -1;
		last_key = -1;
	}
};

