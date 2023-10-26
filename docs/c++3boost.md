# Boost

## [Boost.Serialization](https://theboostcpplibraries.com/boost.serialization)

The library Boost.Serialization makes it possible to convert objects in a C++ program to a __sequence of bytes__ that can be saved and loaded to restore the objects. There are different data formats available to define the rules for generating sequences of bytes. All of the formats supported by Boost.Serialization are only intended for use with this library. For example, the XML format developed for Boost.Serialization should not be used to exchange data with programs that do not use Boost.Serialization. The only advantage of the XML format is that it can make debugging easier since C++ objects are saved in a readable format.

- [Archive](https://theboostcpplibraries.com/boost.serialization-archive)
The main concept of Boost.Serialization is the __archive__. An archive is a sequence of bytes that represent serialized C++ objects. Objects can be added to an archive to serialize them and then later loaded from the archive. In order to restore previously saved C++ objects, the same types are presumed.

Boost.Serialization provides archive classes such as `boost::archive::text_oarchive`, which is defined in `boost/archive/text_oarchive.hpp`. This class makes it possible to serialize objects as a text stream.


To serialize objects of type `std::string,` include the header file `boost/serialization/string.hpp` and to serialize objects of type `std::vector`, include the header file `boost/serialization/vector.hpp`. It is fairly obvious which header file to include.

We can found other types from the directory: `/usr/include/boost/serialization/`:
  - array.hpp
  - bitset.hpp
  - deque.hpp
  - hash_map
  - hash_set
  - stack.hpp
  - scoped_ptr.hpp
  - shared_ptr.hpp
  - unique_ptr.hpp
  - unordered_map.hpp
  - unordered_set.hpp
  - ...


- Pointers and References
Boost.Serialization can also serialize pointers and references. Because a pointer stores the address of an object, serializing the address does not make much sense. When serializing pointers and references, the referenced object is serialized.


