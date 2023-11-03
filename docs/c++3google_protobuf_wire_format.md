# WireFormat

The WireFormat class contains code for implementing the binary protocol buffer wire format via reflection.

The WireFormatLite class implements the non-reflection based routines.

- src/google/protobuf/wire_format_lite.h

- src/google/protobuf/wire_format.h
```c++
  // This is meant for internal protobuf use (WireFormat is an internal class).
  // This is the reflective implementation of the _InternalParse functionality.
  static const char* _InternalParse(Message* msg, const char* ptr, internal::ParseContext* ctx);

    // Parses the value from the wire that belongs to tag.
  static const char* _InternalParseAndMergeField(Message* msg, const char* ptr,
                                                 internal::ParseContext* ctx,
                                                 uint64 tag,
                                                 const Reflection* reflection,
                                                 const FieldDescriptor* field);

```

- src/google/protobuf/wire_format.cc
```c++
const char* WireFormat::_InternalParse(Message* msg, const char* ptr,
                                       internal::ParseContext* ctx) {
  const Descriptor* descriptor = msg->GetDescriptor();
  const Reflection* reflection = msg->GetReflection();
  GOOGLE_DCHECK(descriptor);
  GOOGLE_DCHECK(reflection);
  // message_set_wire_format always false for 
  // https://github.com/protocolbuffers/protobuf/blob/main/src/google/protobuf/descriptor.proto
  // Set true to use the old proto1 MessageSet wire format for extensions.
  // This is provided for backwards-compatibility with the MessageSet wire
  // format.  You should not use this for any other reason:  It's less
  // efficient, has fewer features, and is more complicated.
  if (descriptor->options().message_set_wire_format()) {
    MessageSetParser message_set{msg, descriptor, reflection};
    return message_set.ParseMessageSet(ptr, ctx);
  }
  while (!ctx->Done(&ptr)) { // recall procedure
    uint32 tag;
    ptr = ReadTag(ptr, &tag); // First, ReadTag
    
    if (PROTOBUF_PREDICT_FALSE(ptr == nullptr)) return nullptr;
    if (tag == 0 || (tag & 7) == WireFormatLite::WIRETYPE_END_GROUP) {
      ctx->SetLastTag(tag);
      break;
    }
    const FieldDescriptor* field = nullptr;

    int field_number = WireFormatLite::GetTagFieldNumber(tag);
    field = descriptor->FindFieldByNumber(field_number);

    // If that failed, check if the field is an extension.
    if (field == nullptr && descriptor->IsExtensionNumber(field_number)) {
      if (ctx->data().pool == nullptr) { // What's the meaning of pool?
        field = reflection->FindKnownExtensionByNumber(field_number);
      } else {
        field = ctx->data().pool->FindExtensionByNumber(descriptor, field_number);
      }
    }

    ptr = _InternalParseAndMergeField(msg, ptr, ctx, tag, reflection, field);
    if (PROTOBUF_PREDICT_FALSE(ptr == nullptr)) return nullptr;
  }
  return ptr;
}
```

- _InternalParseAndMergeField
```c++
const char* WireFormat::_InternalParseAndMergeField(Message* msg, const char* ptr, internal::ParseContext* ctx,
                                                    uint64 tag, const Reflection* reflection,
                                                    const FieldDescriptor* field) {
    if (field == nullptr) {
        // unknown field set parser takes 64bit tags, because message set type ids
        // span the full 32 bit range making the tag span [0, 2^35) range.
        return internal::UnknownFieldParse(tag, reflection->MutableUnknownFields(msg), ptr, ctx);
    }
    if (WireFormatLite::GetTagWireType(tag) != WireTypeForFieldType(field->type())) {
        if (field->is_packable() && WireFormatLite::GetTagWireType(tag) == WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
            switch (field->type()) {
#define HANDLE_PACKED_TYPE(TYPE, CPPTYPE, CPPTYPE_METHOD)                                                             \
    case FieldDescriptor::TYPE_##TYPE: {                                                                              \
        ptr = internal::Packed##CPPTYPE_METHOD##Parser(reflection->MutableRepeatedFieldInternal<CPPTYPE>(msg, field), \
                                                       ptr, ctx);                                                     \
        return ptr;                                                                                                   \
    }

                HANDLE_PACKED_TYPE(INT32, int32, Int32)
                HANDLE_PACKED_TYPE(INT64, int64, Int64)
                HANDLE_PACKED_TYPE(SINT32, int32, SInt32)
                HANDLE_PACKED_TYPE(SINT64, int64, SInt64)
                HANDLE_PACKED_TYPE(UINT32, uint32, UInt32)
                HANDLE_PACKED_TYPE(UINT64, uint64, UInt64)

                HANDLE_PACKED_TYPE(FIXED32, uint32, Fixed32)
                HANDLE_PACKED_TYPE(FIXED64, uint64, Fixed64)
                HANDLE_PACKED_TYPE(SFIXED32, int32, SFixed32)
                HANDLE_PACKED_TYPE(SFIXED64, int64, SFixed64)

                HANDLE_PACKED_TYPE(FLOAT, float, Float)
                HANDLE_PACKED_TYPE(DOUBLE, double, Double)

                HANDLE_PACKED_TYPE(BOOL, bool, Bool)
#undef HANDLE_PACKED_TYPE

                case FieldDescriptor::TYPE_ENUM: {
                    auto rep_enum = reflection->MutableRepeatedFieldInternal<int>(msg, field);
                    bool open_enum = false;
                    if (field->file()->syntax() == FileDescriptor::SYNTAX_PROTO3 || open_enum) {
                        ptr = internal::PackedEnumParser(rep_enum, ptr, ctx);
                    } else {
                        return ctx->ReadPackedVarint(ptr, [rep_enum, field, reflection, msg](uint64 val) {
                            if (field->enum_type()->FindValueByNumber(val) != nullptr) {
                                rep_enum->Add(val);
                            } else {
                                WriteVarint(field->number(), val, reflection->MutableUnknownFields(msg));
                            }
                        });
                    }
                    return ptr;
                }

                case FieldDescriptor::TYPE_STRING:
                case FieldDescriptor::TYPE_GROUP:
                case FieldDescriptor::TYPE_MESSAGE:
                case FieldDescriptor::TYPE_BYTES:
                    GOOGLE_LOG(FATAL) << "Can't reach";
                    return nullptr;
            }
        } else {
            // mismatched wiretype;
            return internal::UnknownFieldParse(tag, reflection->MutableUnknownFields(msg), ptr, ctx);
        }
    }

    // Non-packed value
    bool utf8_check = false;
    bool strict_utf8_check = false;
    switch (field->type()) {
#define HANDLE_TYPE(TYPE, CPPTYPE, CPPTYPE_METHOD)              \
    case FieldDescriptor::TYPE_##TYPE: {                        \
        CPPTYPE value;                                          \
        ptr = VarintParse(ptr, &value);                         \
        if (ptr == nullptr) return nullptr;                     \
        if (field->is_repeated()) {                             \
            reflection->Add##CPPTYPE_METHOD(msg, field, value); \
        } else {                                                \
            reflection->Set##CPPTYPE_METHOD(msg, field, value); \
        }                                                       \
        return ptr;                                             \
    }

        HANDLE_TYPE(BOOL, uint64, Bool)
        HANDLE_TYPE(INT32, uint32, Int32)
        HANDLE_TYPE(INT64, uint64, Int64)
        HANDLE_TYPE(UINT32, uint32, UInt32)
        HANDLE_TYPE(UINT64, uint64, UInt64)

        case FieldDescriptor::TYPE_SINT32: {
            int32 value = ReadVarintZigZag32(&ptr);
            if (ptr == nullptr) return nullptr;
            if (field->is_repeated()) {
                reflection->AddInt32(msg, field, value);
            } else {
                reflection->SetInt32(msg, field, value);
            }
            return ptr;
        }
        case FieldDescriptor::TYPE_SINT64: {
            int64 value = ReadVarintZigZag64(&ptr);
            if (ptr == nullptr) return nullptr;
            if (field->is_repeated()) {
                reflection->AddInt64(msg, field, value);
            } else {
                reflection->SetInt64(msg, field, value);
            }
            return ptr;
        }
#undef HANDLE_TYPE
#define HANDLE_TYPE(TYPE, CPPTYPE, CPPTYPE_METHOD)              \
    case FieldDescriptor::TYPE_##TYPE: {                        \
        CPPTYPE value;                                          \
        value = UnalignedLoad<CPPTYPE>(ptr);                    \
        ptr += sizeof(CPPTYPE);                                 \
        if (field->is_repeated()) {                             \
            reflection->Add##CPPTYPE_METHOD(msg, field, value); \
        } else {                                                \
            reflection->Set##CPPTYPE_METHOD(msg, field, value); \
        }                                                       \
        return ptr;                                             \
    }

            HANDLE_TYPE(FIXED32, uint32, UInt32)
            HANDLE_TYPE(FIXED64, uint64, UInt64)
            HANDLE_TYPE(SFIXED32, int32, Int32)
            HANDLE_TYPE(SFIXED64, int64, Int64)

            HANDLE_TYPE(FLOAT, float, Float)
            HANDLE_TYPE(DOUBLE, double, Double)

#undef HANDLE_TYPE

        case FieldDescriptor::TYPE_ENUM: {
            uint32 value;
            ptr = VarintParse(ptr, &value);
            if (ptr == nullptr) return nullptr;
            if (field->is_repeated()) {
                reflection->AddEnumValue(msg, field, value);
            } else {
                reflection->SetEnumValue(msg, field, value);
            }
            return ptr;
        }

        // Handle strings separately so that we can optimize the ctype=CORD case.
        case FieldDescriptor::TYPE_STRING:
            utf8_check = true;
            strict_utf8_check = StrictUtf8Check(field);
            PROTOBUF_FALLTHROUGH_INTENDED;
        case FieldDescriptor::TYPE_BYTES: {
            int size = ReadSize(&ptr);
            if (ptr == nullptr) return nullptr;
            std::string value;
            ptr = ctx->ReadString(ptr, size, &value);
            if (ptr == nullptr) return nullptr;
            if (utf8_check) {
                if (strict_utf8_check) {
                    if (!WireFormatLite::VerifyUtf8String(value.data(), value.length(), WireFormatLite::PARSE,
                                                          field->full_name().c_str())) {
                        return nullptr;
                    }
                } else {
                    VerifyUTF8StringNamedField(value.data(), value.length(), PARSE, field->full_name().c_str());
                }
            }
            if (field->is_repeated()) {
                reflection->AddString(msg, field, value);
            } else {
                reflection->SetString(msg, field, value);
            }
            return ptr;
        }

        case FieldDescriptor::TYPE_GROUP: {
            Message* sub_message;
            if (field->is_repeated()) {
                sub_message = reflection->AddMessage(msg, field, ctx->data().factory);
            } else {
                sub_message = reflection->MutableMessage(msg, field, ctx->data().factory);
            }

            return ctx->ParseGroup(sub_message, ptr, tag);
        }

        case FieldDescriptor::TYPE_MESSAGE: {
            Message* sub_message;
            if (field->is_repeated()) {
                sub_message = reflection->AddMessage(msg, field, ctx->data().factory);
            } else {
                sub_message = reflection->MutableMessage(msg, field, ctx->data().factory);
            }
            return ctx->ParseMessage(sub_message, ptr);
        }
    }

    // GCC 8 complains about control reaching end of non-void function here.
    // Let's keep it happy by returning a nullptr.
    return nullptr;
}
```

## Examples
- modules/google_protobuf/test_builtin_wire_format_lite.cpp

