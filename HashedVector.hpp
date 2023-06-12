#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <algorithm>

//a data structure that maintains a contiguous array of objects and a map of pointers to them
template <typename T>
class HashedVector {

private:
	//a pointer wrapper to check for deletions
	struct _HashObject {
		bool deleted = false;
		bool culled = false;
		T* t;
	};
	//a map to link IDs to t pointers
	std::unordered_map<int, _HashObject> hashMap;
	//the contiguous vector of underlying ts
	std::vector<T> tVector;
	//id counter
	int next_ID = 0;
	// bool to check if a cull is needed before rehash
	bool cullNeeded = false;
	//fix hash object pointers when vector reallocates
	void Rehash()
	{
		if (cullNeeded)
			CullDeleted();
		int check_ID = 0;
		for (int i = 0; i < tVector.size(); i++)
		{
			//ignored deleted ID's
			while (hashMap[check_ID].deleted)
			{
				check_ID++;
			}
			//assign current address to has object
			hashMap[check_ID].t = &tVector[i];
			//increment ID
			check_ID++;
		}
	}

	//internal function to remove a single element from the vector, not error checked
	void RemoveElementFromVector(int id)
	{
		T* elementToRemove = hashMap[id].t;
		auto matchElement = [elementToRemove](const T& e) {
			return &e == elementToRemove;};
		auto eraseIt = std::find_if(tVector.begin(), tVector.end(), matchElement);
		if (eraseIt != tVector.end()) {
			tVector.erase(eraseIt);
		}
		hashMap[id].culled = true;
	}
public:
	//adds the item to the vector and returns its key ID
	int push_back(T t)
	{
		//check vector capacity
		int capacity_before = tVector.capacity();
		//set ID
		int ID = next_ID;
		//increment next ID
		next_ID++;
		//Add it to the vector
		tVector.push_back(t);
		_HashObject h;
		h.t = &tVector.back();
		//Add object to hashMap;
		hashMap[ID] = h;
		//rehash if vector capacity changed
		if (tVector.capacity() != capacity_before)
			Rehash();
		return ID;
	}

	//removes the item from the vector, cull wil be performed on rehash if set to false
	void remove(int id, bool cullVector = true)
	{
		//check if ID is valid
		if (hashMap.find(id) == hashMap.end())
			throw std::out_of_range("Invalid index assertion error: Index out of range.");
		//check if ID is deleted
		if (hashMap[id].deleted)
			return;
		//check if pointer is valid
		if (hashMap[id].t == nullptr)
			return;
		// Mark object as deleted
		hashMap[id].deleted = true;

		cullNeeded = true;
		if (cullVector)
			CullDeleted();
	}

	//cull deleted elements from the vector to maintain contiguousness
	void CullDeleted()
	{
		//check if cull is needed
		if (!cullNeeded)
			return;

		// Iterate through the hash map and remove elements with element.deleted = true
		for (auto it = hashMap.begin(); it != hashMap.end(); it++) {
			if (it->second.deleted && !it->second.culled)
				RemoveElementFromVector(it->first);
		}

		// Reset cullNeeded flag
		cullNeeded = false;
		//rehash
		Rehash();
	}


	//returns the item at the given index, will throw error if invalid
	T& operator [](int id)
	{
		auto it = hashMap.find(id);
		if (it == hashMap.end())
			throw std::out_of_range("Invalid index assertion error: Index out of range.");
		if (hashMap[id].deleted)
			throw std::out_of_range("Invalid index assertion error: element deleted.");
		return *hashMap[id].t;

	}

	//returns the item at the given index, will return nullptr if invalid
	T* try_get(int id)
	{
		auto it = hashMap.find(id);
		if (it == hashMap.end())
			return nullptr;
		if (hashMap[id].deleted)
			return nullptr;
		return hashMap[id].t;
	}

	void reserve(int size)
	{
		tVector.reserve(size);
		Rehash();
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

	//returns a reference to the underlying vector
	std::vector<T>& getVector()
	{
		if (cullNeeded)
			CullDeleted();
		return tVector;
	}



};
