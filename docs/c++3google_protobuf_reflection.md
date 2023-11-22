# 反射有三个重要的类：

- Descriptor： 这个类是对应一种 Message 类型的，相同类型 Message 的 Descriptor 是一样的。通过这个类可以获取 Message 有多少个字段，以及获取字段的 FieldDescriptor

- FieldDescriptor： 这个类是用来描述某个具体字段相关的信息的，包括字段的类型、名称、编号等

- Reflection： 通过它和 FieldDescriptor，我们就能实现对具体字段的读或者写


- src/google/protobuf/descriptor_unittest.cc


## Descriptor


- Descriptor: src/google/protobuf/descriptor.h

```c++
// Describes a type of protocol message, or a particular group within a
// message.  To obtain the Descriptor for a given message object, call
// Message::GetDescriptor().  Generated message classes also have a
// static method called descriptor() which returns the type's descriptor.
// Use DescriptorPool to construct your own descriptors.
class PROTOBUF_EXPORT Descriptor {
 public:
  typedef DescriptorProto Proto;
};

```

- LazyDescriptor
```c++
// A class to handle the simplest cases of a lazily linked descriptor
// for a message type that isn't built at the time of cross linking,
// which is needed when a pool has lazily_build_dependencies_ set.
// Must be instantiated as mutable in a descriptor.
namespace internal {
class PROTOBUF_EXPORT LazyDescriptor {
```

- FieldDescriptor: src/google/protobuf/descriptor.h
```c++
// Describes a single field of a message.  To get the descriptor for a given
// field, first get the Descriptor for the message in which it is defined,
// then call Descriptor::FindFieldByName().  To get a FieldDescriptor for
// an extension, do one of the following:
// - Get the Descriptor or FileDescriptor for its containing scope, then
//   call Descriptor::FindExtensionByName() or
//   FileDescriptor::FindExtensionByName().
// - Given a DescriptorPool, call DescriptorPool::FindExtensionByNumber() or
//   DescriptorPool::FindExtensionByPrintableName().
// Use DescriptorPool to construct your own descriptors.
class PROTOBUF_EXPORT FieldDescriptor {
 public:
  typedef FieldDescriptorProto Proto;

  // Identifies a field type.  0 is reserved for errors.  The order is weird
  // for historical reasons.  Types 12 and up are new in proto2.
  enum Type {
    TYPE_DOUBLE = 1,    // double, exactly eight bytes on the wire.
    TYPE_FLOAT = 2,     // float, exactly four bytes on the wire.
    TYPE_INT64 = 3,     // int64, varint on the wire.  Negative numbers
                        // take 10 bytes.  Use TYPE_SINT64 if negative
                        // values are likely.
    TYPE_UINT64 = 4,    // uint64, varint on the wire.
    TYPE_INT32 = 5,     // int32, varint on the wire.  Negative numbers
                        // take 10 bytes.  Use TYPE_SINT32 if negative
                        // values are likely.
    TYPE_FIXED64 = 6,   // uint64, exactly eight bytes on the wire.
    TYPE_FIXED32 = 7,   // uint32, exactly four bytes on the wire.
    TYPE_BOOL = 8,      // bool, varint on the wire.
    TYPE_STRING = 9,    // UTF-8 text.
    TYPE_GROUP = 10,    // Tag-delimited message.  Deprecated.
    TYPE_MESSAGE = 11,  // Length-delimited message.

    TYPE_BYTES = 12,     // Arbitrary byte array.
    TYPE_UINT32 = 13,    // uint32, varint on the wire
    TYPE_ENUM = 14,      // Enum, varint on the wire
    TYPE_SFIXED32 = 15,  // int32, exactly four bytes on the wire
    TYPE_SFIXED64 = 16,  // int64, exactly eight bytes on the wire
    TYPE_SINT32 = 17,    // int32, ZigZag-encoded varint on the wire
    TYPE_SINT64 = 18,    // int64, ZigZag-encoded varint on the wire

    MAX_TYPE = 18,  // Constant useful for defining lookup tables
                    // indexed by Type.
  };
```

- src/google/protobuf/message.h
```c++
// This interface contains methods that can be used to dynamically access
// and modify the fields of a protocol message.  Their semantics are
// similar to the accessors the protocol compiler generates.
//
// To get the Reflection for a given Message, call Message::GetReflection().
//
// This interface is separate from Message only for efficiency reasons;
// the vast majority of implementations of Message will share the same
// implementation of Reflection (GeneratedMessageReflection,
// defined in generated_message.h), and all Messages of a particular class
// should share the same Reflection object (though you should not rely on
// the latter fact).
//
// There are several ways that these methods can be used incorrectly.  For
// example, any of the following conditions will lead to undefined
// results (probably assertion failures):
// - The FieldDescriptor is not a field of this message type.
// - The method called is not appropriate for the field's type.  For
//   each field type in FieldDescriptor::TYPE_*, there is only one
//   Get*() method, one Set*() method, and one Add*() method that is
//   valid for that type.  It should be obvious which (except maybe
//   for TYPE_BYTES, which are represented using strings in C++).
// - A Get*() or Set*() method for singular fields is called on a repeated
//   field.
// - GetRepeated*(), SetRepeated*(), or Add*() is called on a non-repeated
//   field.
// - The Message object passed to any method is not of the right type for
//   this Reflection object (i.e. message.GetReflection() != reflection).
//
// You might wonder why there is not any abstract representation for a field
// of arbitrary type.  E.g., why isn't there just a "GetField()" method that
// returns "const Field&", where "Field" is some class with accessors like
// "GetInt32Value()".  The problem is that someone would have to deal with
// allocating these Field objects.  For generated message classes, having to
// allocate space for an additional object to wrap every field would at least
// double the message's memory footprint, probably worse.  Allocating the
// objects on-demand, on the other hand, would be expensive and prone to
// memory leaks.  So, instead we ended up with this flat interface.
class PROTOBUF_EXPORT Reflection final {
```

## DynamicMessage

- src/google/protobuf/dynamic_message.h

```c++
namespace google {
namespace protobuf {

// Defined in other files.
class Descriptor;      // descriptor.h
class DescriptorPool;  // descriptor.h

// Constructs implementations of Message which can emulate types which are not
// known at compile-time.
//
// Sometimes you want to be able to manipulate protocol types that you don't
// know about at compile time.  It would be nice to be able to construct
// a Message object which implements the message type given by any arbitrary
// Descriptor.  DynamicMessage provides this.
//
// As it turns out, a DynamicMessage needs to construct extra
// information about its type in order to operate.  Most of this information
// can be shared between all DynamicMessages of the same type.  But, caching
// this information in some sort of global map would be a bad idea, since
// the cached information for a particular descriptor could outlive the
// descriptor itself.  To avoid this problem, DynamicMessageFactory
// encapsulates this "cache".  All DynamicMessages of the same type created
// from the same factory will share the same support data.  Any Descriptors
// used with a particular factory must outlive the factory.
class PROTOBUF_EXPORT DynamicMessageFactory : public MessageFactory {
``

- src/google/protobuf/dynamic_message_unittest.cc

- Q: 

Basically I'm trying to work out how to deserialize a protobuf message without using the generated headers, as we're likely to get messages that weren't generated at compile time. I've looked through the documentation, but I only seem to be able to find ones that use generated classes to deserialize, or that use a Descriptor from a generated class to create a DynamicMessage, which I can't seem to work out how to do when we don't have the proto.

- A:
The protobuf encoding isn't self-describing; generally you need either a compiled message definition or a Descriptor to make sense of an encoded message.
Without one of those, there is not much you can do beyond splitting the message into opaque fields that you will have difficulty in interpreting further.

As Descriptors can be turned into protobuf messages, one approach is to include a serialized Descriptor as part of the standard message framing; then you can use DynamicMessage on the receiver side.
See https://developers.google.com/protocol-buffers/docs/techniques#self-description


## DescriptorDatabase
- src/google/protobuf/descriptor_database.h

// Abstract interface for a database of descriptors.
//
// This is useful if you want to create a DescriptorPool which loads
// descriptors on-demand from some sort of large database.  If the database
// is large, it may be inefficient to enumerate every .proto file inside it
// calling DescriptorPool::BuildFile() for each one.  Instead, a DescriptorPool
// can be created which wraps a DescriptorDatabase and only builds particular
// descriptors when they are needed.