#ifndef HASHEDVECTOR_HPP
#define HASHEDVECTOR_HPP

#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <algorithm>

template <typename T>
class HashedVector {
private:
	struct _HashObject {
		bool deleted = false;
		int vector_index = -1;
	};

	std::unordered_map<int, _HashObject> hashMap;
	std::vector<T> tVector;
	int next_ID = 0;
	bool cullNeeded = false;

	void Rehash()
	{
		if (cullNeeded)
			CullDeleted();
		int index = 0;
		for (auto it = hashMap.begin(); it != hashMap.end(); ++it)
		{
			if (it->second.deleted)
			{
				continue;
			}
			it->second.vector_index = index;
			index++;
		}
	}

public:
	//pass constructor arguments
	template <typename... Args>
	int emplace_back(Args&&... args)
	{
		int capacity_before = tVector.capacity();
		int ID = next_ID;
		next_ID++;
		tVector.emplace_back(std::forward<Args>(args)...);
		hashMap[ID].vector_index = tVector.size() - 1;
		if (tVector.capacity() != capacity_before)
			Rehash();
		return ID;
	}

	int push_back(const T& t)
	{
		int capacity_before = tVector.capacity();
		int ID = next_ID;
		next_ID++;
		tVector.push_back(t);
		hashMap[ID].vector_index = tVector.size() - 1;
		if (tVector.capacity() != capacity_before)
			Rehash();
		return ID;
	}

	void remove(const int& id, const bool& cullVector = true)
	{
		auto it = hashMap.find(id);
		if (it != hashMap.end())
		{
			it->second.deleted = true;
			cullNeeded = true;
			if (cullVector)
				CullDeleted();
		}
	}

	void CullDeleted()
	{
		if (!cullNeeded)
			return;

		int elementsRemoved = 0;
		for (auto it = hashMap.begin(); it != hashMap.end();)
		{
			if (it->second.deleted)
			{
				tVector.erase(tVector.begin() + (it->second.vector_index - elementsRemoved));
				it = hashMap.erase(it);
				++elementsRemoved;
			}
			else
			{
				++it;
			}
		}
		cullNeeded = false;
		Rehash();
	}


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
		next_ID = 0;
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
