# Hashed_Vector_CPP

As I have been working on a component based game engine I realised I would both like to query a data structure based on ID and maintain contiguousness.

This is especially useful when passing a raw pointer to a buffer such as a Vertex Buffer Object. I neither want a sparse array nor poor data locality.

This is my attempt to resolve this issue. An unordered map keeps track of ID's and whether or not underlying data in the vector is marked for deletion. As the vector is dynamically sized, pointer invalidation would usually occur. A check performs a cull, deleting data marked for deletion then a rehash is performed ensuring that the hash pointers are updated.

Many of the vector functions are simply redirected through the wrapper with the exception of getVector which first performs a cull and rehash before returning a reference to the vector.

Unnecessary reallocation can be avoided using the reserve() function as in a vector. Unecessary rehashing can be avoided by explicity marking remove() with a noCull flag which will leave the underlying data intact in the vector until culled manually or reallocation is performed by the vector.

For my use case I will probably allow entities to store ID's for attached components that can be queried via a template function attached to the entity.

