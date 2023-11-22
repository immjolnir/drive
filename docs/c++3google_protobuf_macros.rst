======
Macros
======

google protobuf's macro instroduce


POTOBUF_DEPRECATED_MSG
======================

From: src/google/protobuf/message.h

Usage:

    // DEPRECATED. Please use GetMutableRepeatedFieldRef().
    //
    // for T = Cord and all protobuf scalar types except enums.
    template <typename T>
    PROTOBUF_DEPRECATED_MSG("Please use GetMutableRepeatedFieldRef() instead")
    RepeatedField<T>* MutableRepeatedField(Message* msg, const FieldDescriptor* d) const {
      return MutableRepeatedFieldInternal<T>(msg, d);
    }


Definition:

    #if defined(__GNUC__) || defined(__clang__)
    #define PROTOBUF_DEPRECATED __attribute__((deprecated))
    #define PROTOBUF_DEPRECATED_ENUM __attribute__((deprecated))
    #define PROTOBUF_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
    #elif defined(_MSC_VER)
    #define PROTOBUF_DEPRECATED __declspec(deprecated)
    #define PROTOBUF_DEPRECATED_ENUM
    #define PROTOBUF_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
    #endif


