#if defined(_MSC_VER)
  #pragma warning (disable:4018) // 'expression' : signed/unsigned mismatch
  #pragma warning (disable:4065) // switch statement contains 'default' but no 'case' labels
  #pragma warning (disable:4146) // unary minus operator applied to unsigned type, result still unsigned
  #pragma warning (disable:4244) // 'conversion' conversion from 'type1' to 'type2', possible loss of data
  #pragma warning (disable:4251) // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
  #pragma warning (disable:4267) // 'var' : conversion from 'size_t' to 'type', possible loss of data
  #pragma warning (disable:4305) // 'identifier' : truncation from 'type1' to 'type2'
  #pragma warning (disable:4307) // 'operator' : integral constant overflow
  #pragma warning (disable:4309) // 'conversion' : truncation of constant value
  #pragma warning (disable:4334) // 'operator' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
  #pragma warning (disable:4355) // 'this' : used in base member initializer list
  #pragma warning (disable:4506) // no definition for inline function 'function'
  #pragma warning (disable:4996) // The compiler encountered a deprecated declaration.
  #pragma warning (disable:4125) // decimal digit terminates octal escape sequence
#endif
// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: sro/filter.proto

#include "sro/filter.pb.h"

#include <algorithm>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = ::PROTOBUF_NAMESPACE_ID::internal;
namespace sro {
template <typename>
PROTOBUF_CONSTEXPR QueryFilters::QueryFilters(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.limit_)*/ 0

  , /*decltype(_impl_.offset_)*/ 0

  , /*decltype(_impl_._cached_size_)*/{}} {}
struct QueryFiltersDefaultTypeInternal {
  PROTOBUF_CONSTEXPR QueryFiltersDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~QueryFiltersDefaultTypeInternal() {}
  union {
    QueryFilters _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 QueryFiltersDefaultTypeInternal _QueryFilters_default_instance_;
}  // namespace sro
static ::_pb::Metadata file_level_metadata_sro_2ffilter_2eproto[1];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_sro_2ffilter_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_sro_2ffilter_2eproto = nullptr;
const ::uint32_t TableStruct_sro_2ffilter_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::sro::QueryFilters, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::sro::QueryFilters, _impl_.limit_),
    PROTOBUF_FIELD_OFFSET(::sro::QueryFilters, _impl_.offset_),
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        { 0, -1, -1, sizeof(::sro::QueryFilters)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::sro::_QueryFilters_default_instance_._instance,
};
const char descriptor_table_protodef_sro_2ffilter_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\020sro/filter.proto\022\003sro\"-\n\014QueryFilters\022"
    "\r\n\005limit\030\001 \001(\005\022\016\n\006offset\030\002 \001(\005B8Z6github"
    ".com/ShatteredRealms/go-common-service/p"
    "kg/pb;pbb\006proto3"
};
static ::absl::once_flag descriptor_table_sro_2ffilter_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_sro_2ffilter_2eproto = {
    false,
    false,
    136,
    descriptor_table_protodef_sro_2ffilter_2eproto,
    "sro/filter.proto",
    &descriptor_table_sro_2ffilter_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_sro_2ffilter_2eproto::offsets,
    file_level_metadata_sro_2ffilter_2eproto,
    file_level_enum_descriptors_sro_2ffilter_2eproto,
    file_level_service_descriptors_sro_2ffilter_2eproto,
};

// This function exists to be marked as weak.
// It can significantly speed up compilation by breaking up LLVM's SCC
// in the .pb.cc translation units. Large translation units see a
// reduction of more than 35% of walltime for optimized builds. Without
// the weak attribute all the messages in the file, including all the
// vtables and everything they use become part of the same SCC through
// a cycle like:
// GetMetadata -> descriptor table -> default instances ->
//   vtables -> GetMetadata
// By adding a weak function here we break the connection from the
// individual vtables back into the descriptor table.
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_sro_2ffilter_2eproto_getter() {
  return &descriptor_table_sro_2ffilter_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_sro_2ffilter_2eproto(&descriptor_table_sro_2ffilter_2eproto);
namespace sro {
// ===================================================================

class QueryFilters::_Internal {
 public:
};

QueryFilters::QueryFilters(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:sro.QueryFilters)
}
QueryFilters::QueryFilters(const QueryFilters& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(), _impl_(from._impl_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(
      from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:sro.QueryFilters)
}

inline void QueryFilters::SharedCtor(::_pb::Arena* arena) {
  (void)arena;
  new (&_impl_) Impl_{
      decltype(_impl_.limit_) { 0 }

    , decltype(_impl_.offset_) { 0 }

    , /*decltype(_impl_._cached_size_)*/{}
  };
}

QueryFilters::~QueryFilters() {
  // @@protoc_insertion_point(destructor:sro.QueryFilters)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void QueryFilters::SharedDtor() {
  ABSL_DCHECK(GetArenaForAllocation() == nullptr);
}

void QueryFilters::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void QueryFilters::Clear() {
// @@protoc_insertion_point(message_clear_start:sro.QueryFilters)
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&_impl_.limit_, 0, static_cast<::size_t>(
      reinterpret_cast<char*>(&_impl_.offset_) -
      reinterpret_cast<char*>(&_impl_.limit_)) + sizeof(_impl_.offset_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* QueryFilters::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 limit = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 8)) {
          _impl_.limit_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      // int32 offset = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 16)) {
          _impl_.offset_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else {
          goto handle_unusual;
        }
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

::uint8_t* QueryFilters::_InternalSerialize(
    ::uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sro.QueryFilters)
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 limit = 1;
  if (this->_internal_limit() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(
        1, this->_internal_limit(), target);
  }

  // int32 offset = 2;
  if (this->_internal_offset() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(
        2, this->_internal_offset(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sro.QueryFilters)
  return target;
}

::size_t QueryFilters::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sro.QueryFilters)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 limit = 1;
  if (this->_internal_limit() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_limit());
  }

  // int32 offset = 2;
  if (this->_internal_offset() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
        this->_internal_offset());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData QueryFilters::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    QueryFilters::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*QueryFilters::GetClassData() const { return &_class_data_; }


void QueryFilters::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<QueryFilters*>(&to_msg);
  auto& from = static_cast<const QueryFilters&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:sro.QueryFilters)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_limit() != 0) {
    _this->_internal_set_limit(from._internal_limit());
  }
  if (from._internal_offset() != 0) {
    _this->_internal_set_offset(from._internal_offset());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void QueryFilters::CopyFrom(const QueryFilters& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sro.QueryFilters)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool QueryFilters::IsInitialized() const {
  return true;
}

void QueryFilters::InternalSwap(QueryFilters* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(QueryFilters, _impl_.offset_)
      + sizeof(QueryFilters::_impl_.offset_)
      - PROTOBUF_FIELD_OFFSET(QueryFilters, _impl_.limit_)>(
          reinterpret_cast<char*>(&_impl_.limit_),
          reinterpret_cast<char*>(&other->_impl_.limit_));
}

::PROTOBUF_NAMESPACE_ID::Metadata QueryFilters::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_sro_2ffilter_2eproto_getter, &descriptor_table_sro_2ffilter_2eproto_once,
      file_level_metadata_sro_2ffilter_2eproto[0]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace sro
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::sro::QueryFilters*
Arena::CreateMaybeMessage< ::sro::QueryFilters >(Arena* arena) {
  return Arena::CreateMessageInternal< ::sro::QueryFilters >(arena);
}
PROTOBUF_NAMESPACE_CLOSE
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
