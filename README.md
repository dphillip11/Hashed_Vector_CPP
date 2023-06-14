# Hashed_Vector_CPP

As I have been working on a component based game engine I realised I would both like to query a data structure based on ID and maintain contiguousness.

This is especially useful when passing a raw pointer to a buffer such as a Vertex Buffer Object. I neither want a sparse array nor poor data locality.

This is my attempt to resolve this issue. An unordered map keeps track of vector indices and reroutes you to the correct index. 

Many of the vector functions are simply redirected through the wrapper.

For my use case I will probably allow entities to store ID's for attached components that can be queried via a template function attached to the entity.

## Usage

~~~~
#include "HashedVector.hpp"

HashedVector<float> hv;

hv.reserve(100);

int ID = hv.push_back(3.0);

hv[ID] = 5;

hv.remove(ID);
  
const auto& vector = hv.getVector();
~~~~
## Summary

* Allows for an underlying vector of data with an ID lookup feature.
* This overcomes pointer invalidation and allows deletion from a vector while maintaining lookups.
* ID's remain constant.
* Indexing a Hashed_Vector is an ID lookup, it will return an error for invalid or deleted ID's. 
* Use try_get(ID) to check for existing entry.  
* You can still access the underlying vector but adding or removing entries this way will corrupt the hashed vector.
  

  

  

  


