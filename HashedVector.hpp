#ifndef HASHEDVECTOR_HPP
#define HASHEDVECTOR_HPP

#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>

template <typename T>
 class HashedVector{
	struct _HashObject {
		bool deleted = false;
		int next_key = -1;
		int prev_key = -1;
		int last_key = -1;
		int vector_index = -1;
	};

	std::map<int, _HashObject> hashMap;
	std::vector<T> tVector;

	int next_ID = 0;
	int first_key = -1;
	int last_key = -1;

	void Rehash()
	{
		int list_iterator = first_key;
		int index = 0;
		while(list_iterator != -1)
		{
			if(!hashMap[list_iterator].deleted)
			{
				hashMap[list_iterator].vector_index = index;
				index++;
			}
			list_iterator = hashMap[list_iterator].next_key;
		}
	}

	void internal_remove_hash_object(int ID)
	{
		auto& hashObject = hashMap[ID];
		if(hashObject.next_key != -1)
			hashMap[hashObject.next_key].prev_key = hashObject.prev_key;
		if(hashObject.prev_key != -1)
			hashMap[hashObject.prev_key].next_key = hashObject.next_key;
		if(ID == first_key)
			first_key = hashObject.next_key;
		if(ID == last_key)
			last_key = hashObject.prev_key;
		hashMap.erase(ID);
	}

public:
	//pass constructor arguments
	template <typename... Args>
	int emplace_back(Args&&... args)
	{
		int capacity_before = tVector.capacity();
		int ID = next_ID;
		++next_ID;
		tVector.emplace_back(std::forward<Args>(args)...);
		hashMap[ID].vector_index = tVector.size() - 1;
		hashMap[ID].prev_key = last_key;
		if (last_key != -1)
			hashMap[last_key].next_key = ID;
		last_key = ID;
		if (first_key == -1)
			first_key = ID;
		if (tVector.capacity() != capacity_before)
			Rehash();
		return ID;
	}

	int push_back(const T& t)
	{
		int capacity_before = tVector.capacity();
		int ID = next_ID;
		++next_ID;
		tVector.push_back(t);
		hashMap[ID].vector_index = tVector.size() - 1;
		hashMap[ID].prev_key = last_key;
		if (last_key != -1)
			hashMap[last_key].next_key = ID;
		last_key = ID;
		if (first_key == -1)
			first_key = ID;
		if (tVector.capacity() != capacity_before)
			Rehash();
		return ID;
	}

	void remove(const int& id)
	{
		auto it = hashMap.find(id);
		if (it != hashMap.end())
		{
			it->second.deleted = true;
			CullDeleted();
		}
	}

// 	void CullDeleted()
// 	{

// 		int list_iterator = first_key;
// 		auto traverseList = [&]()
// 			{
// 				auto next_iterator = hashMap[list_iterator].next_key;
// 				auto index = hashMap[list_iterator].vector_index;
// 				if(hashMap[list_iterator].deleted)
// 				{
// 					tVector.erase(tVector.begin() + index);
// 					internal_remove_hash_object(list_iterator);
// 				}
// 				list_iterator = next_iterator;
// 			};
// 		while(list_iterator != -1 && hashMap[list_iterator].deleted)
// 		{
// 			traverseList();
// 		}
// 		if (list_iterator == -1)
// 		{
// 			clear();
// 			return;
// 		}
// 		first_key = list_iterator;
// 		//indexing here is bring the entry back to life!!!!!!!!!!!!!
// 		while(list_iterator != -1)
// 		{
// 			traverseList();
// 			last_key = list_iterator;
// 		}
	
// 		if(hashMap[list_iterator].deleted)
// 				{
// 			tVector.erase(tVector.begin() + list_iterator);
// 					internal_remove_hash_object(list_iterator);
// 				}
// 		Rehash();
// }

T& operator[](const int& id)
{
	auto it = hashMap.find(id);
	if (it != hashMap.end())
	{
		if (!it->second.deleted)
			return tVector[it->second.vector_index];
	}
	throw std::out_of_range("Invalid index assertion error: Index out of range.");
}

T* try_get(const int& id)
{
	auto it = hashMap.find(id);
	if (it != hashMap.end())
	{
		if (!it->second.deleted)
			return &(tVector[it->second.vector_index]);
	}
	return nullptr;
}

auto size() const noexcept
{
	return tVector.size();
}

bool empty() const noexcept
{
	return tVector.empty();
}

void clear() noexcept
{
	tVector.clear();
	hashMap.clear();
	first_key = -1;
	last_key = -1;
}

void reserve(const int& size)
{
	tVector.reserve(size);
	Rehash();
}

const std::vector<T>& getVector() const
{
	return tVector;
}
};

#endif // HASHEDVECTOR_HPP
