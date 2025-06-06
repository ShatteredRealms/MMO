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
// source: sro/bus.proto

#include "sro/bus.pb.h"

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
PROTOBUF_CONSTEXPR BusTarget::BusTarget(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.type_)*/ {
    &::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized {}
  }

  , /*decltype(_impl_._cached_size_)*/{}} {}
struct BusTargetDefaultTypeInternal {
  PROTOBUF_CONSTEXPR BusTargetDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~BusTargetDefaultTypeInternal() {}
  union {
    BusTarget _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 BusTargetDefaultTypeInternal _BusTarget_default_instance_;
template <typename>
PROTOBUF_CONSTEXPR ResetBusResponse::ResetBusResponse(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.message_)*/ {
    &::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized {}
  }

  , /*decltype(_impl_._cached_size_)*/{}} {}
struct ResetBusResponseDefaultTypeInternal {
  PROTOBUF_CONSTEXPR ResetBusResponseDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~ResetBusResponseDefaultTypeInternal() {}
  union {
    ResetBusResponse _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 ResetBusResponseDefaultTypeInternal _ResetBusResponse_default_instance_;
}  // namespace sro
static ::_pb::Metadata file_level_metadata_sro_2fbus_2eproto[2];
static constexpr const ::_pb::EnumDescriptor**
    file_level_enum_descriptors_sro_2fbus_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor**
    file_level_service_descriptors_sro_2fbus_2eproto = nullptr;
const ::uint32_t TableStruct_sro_2fbus_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(
    protodesc_cold) = {
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::sro::BusTarget, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::sro::BusTarget, _impl_.type_),
    ~0u,  // no _has_bits_
    PROTOBUF_FIELD_OFFSET(::sro::ResetBusResponse, _internal_metadata_),
    ~0u,  // no _extensions_
    ~0u,  // no _oneof_case_
    ~0u,  // no _weak_field_map_
    ~0u,  // no _inlined_string_donated_
    ~0u,  // no _split_
    ~0u,  // no sizeof(Split)
    PROTOBUF_FIELD_OFFSET(::sro::ResetBusResponse, _impl_.message_),
};

static const ::_pbi::MigrationSchema
    schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
        { 0, -1, -1, sizeof(::sro::BusTarget)},
        { 9, -1, -1, sizeof(::sro::ResetBusResponse)},
};

static const ::_pb::Message* const file_default_instances[] = {
    &::sro::_BusTarget_default_instance_._instance,
    &::sro::_ResetBusResponse_default_instance_._instance,
};
const char descriptor_table_protodef_sro_2fbus_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
    "\n\rsro/bus.proto\022\003sro\032\034google/api/annotat"
    "ions.proto\"\031\n\tBusTarget\022\014\n\004type\030\001 \001(\t\"#\n"
    "\020ResetBusResponse\022\017\n\007message\030\001 \001(\t2\272\001\n\nB"
    "usService\022U\n\016ResetReaderBus\022\016.sro.BusTar"
    "get\032\025.sro.ResetBusResponse\"\034\202\323\344\223\002\026\022\024/v1/"
    "bus/reset/reader\022U\n\016ResetWriterBus\022\016.sro"
    ".BusTarget\032\025.sro.ResetBusResponse\"\034\202\323\344\223\002"
    "\026\022\024/v1/bus/reset/writerB8Z6github.com/Sh"
    "atteredRealms/go-common-service/pkg/pb;p"
    "bb\006proto3"
};
static const ::_pbi::DescriptorTable* const descriptor_table_sro_2fbus_2eproto_deps[1] =
    {
        &::descriptor_table_google_2fapi_2fannotations_2eproto,
};
static ::absl::once_flag descriptor_table_sro_2fbus_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_sro_2fbus_2eproto = {
    false,
    false,
    369,
    descriptor_table_protodef_sro_2fbus_2eproto,
    "sro/bus.proto",
    &descriptor_table_sro_2fbus_2eproto_once,
    descriptor_table_sro_2fbus_2eproto_deps,
    1,
    2,
    schemas,
    file_default_instances,
    TableStruct_sro_2fbus_2eproto::offsets,
    file_level_metadata_sro_2fbus_2eproto,
    file_level_enum_descriptors_sro_2fbus_2eproto,
    file_level_service_descriptors_sro_2fbus_2eproto,
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
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_sro_2fbus_2eproto_getter() {
  return &descriptor_table_sro_2fbus_2eproto;
}
// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2
static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_sro_2fbus_2eproto(&descriptor_table_sro_2fbus_2eproto);
namespace sro {
// ===================================================================

class BusTarget::_Internal {
 public:
};

BusTarget::BusTarget(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:sro.BusTarget)
}
BusTarget::BusTarget(const BusTarget& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  BusTarget* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.type_) {}

    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.type_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        _impl_.type_.Set("", GetArenaForAllocation());
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_type().empty()) {
    _this->_impl_.type_.Set(from._internal_type(), _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:sro.BusTarget)
}

inline void BusTarget::SharedCtor(::_pb::Arena* arena) {
  (void)arena;
  new (&_impl_) Impl_{
      decltype(_impl_.type_) {}

    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.type_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        _impl_.type_.Set("", GetArenaForAllocation());
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

BusTarget::~BusTarget() {
  // @@protoc_insertion_point(destructor:sro.BusTarget)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void BusTarget::SharedDtor() {
  ABSL_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.type_.Destroy();
}

void BusTarget::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void BusTarget::Clear() {
// @@protoc_insertion_point(message_clear_start:sro.BusTarget)
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.type_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* BusTarget::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string type = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_type();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "sro.BusTarget.type"));
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

::uint8_t* BusTarget::_InternalSerialize(
    ::uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sro.BusTarget)
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string type = 1;
  if (!this->_internal_type().empty()) {
    const std::string& _s = this->_internal_type();
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE, "sro.BusTarget.type");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sro.BusTarget)
  return target;
}

::size_t BusTarget::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sro.BusTarget)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string type = 1;
  if (!this->_internal_type().empty()) {
    total_size += 1 + ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
                                    this->_internal_type());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData BusTarget::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    BusTarget::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*BusTarget::GetClassData() const { return &_class_data_; }


void BusTarget::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<BusTarget*>(&to_msg);
  auto& from = static_cast<const BusTarget&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:sro.BusTarget)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_type().empty()) {
    _this->_internal_set_type(from._internal_type());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void BusTarget::CopyFrom(const BusTarget& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sro.BusTarget)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BusTarget::IsInitialized() const {
  return true;
}

void BusTarget::InternalSwap(BusTarget* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.type_, lhs_arena,
                                       &other->_impl_.type_, rhs_arena);
}

::PROTOBUF_NAMESPACE_ID::Metadata BusTarget::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_sro_2fbus_2eproto_getter, &descriptor_table_sro_2fbus_2eproto_once,
      file_level_metadata_sro_2fbus_2eproto[0]);
}
// ===================================================================

class ResetBusResponse::_Internal {
 public:
};

ResetBusResponse::ResetBusResponse(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:sro.ResetBusResponse)
}
ResetBusResponse::ResetBusResponse(const ResetBusResponse& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  ResetBusResponse* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.message_) {}

    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.message_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        _impl_.message_.Set("", GetArenaForAllocation());
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_message().empty()) {
    _this->_impl_.message_.Set(from._internal_message(), _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:sro.ResetBusResponse)
}

inline void ResetBusResponse::SharedCtor(::_pb::Arena* arena) {
  (void)arena;
  new (&_impl_) Impl_{
      decltype(_impl_.message_) {}

    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.message_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        _impl_.message_.Set("", GetArenaForAllocation());
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

ResetBusResponse::~ResetBusResponse() {
  // @@protoc_insertion_point(destructor:sro.ResetBusResponse)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void ResetBusResponse::SharedDtor() {
  ABSL_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.message_.Destroy();
}

void ResetBusResponse::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void ResetBusResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:sro.ResetBusResponse)
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.message_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ResetBusResponse::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    ::uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string message = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_message();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "sro.ResetBusResponse.message"));
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

::uint8_t* ResetBusResponse::_InternalSerialize(
    ::uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:sro.ResetBusResponse)
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string message = 1;
  if (!this->_internal_message().empty()) {
    const std::string& _s = this->_internal_message();
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
        _s.data(), static_cast<int>(_s.length()), ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE, "sro.ResetBusResponse.message");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:sro.ResetBusResponse)
  return target;
}

::size_t ResetBusResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:sro.ResetBusResponse)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string message = 1;
  if (!this->_internal_message().empty()) {
    total_size += 1 + ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
                                    this->_internal_message());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData ResetBusResponse::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    ResetBusResponse::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*ResetBusResponse::GetClassData() const { return &_class_data_; }


void ResetBusResponse::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<ResetBusResponse*>(&to_msg);
  auto& from = static_cast<const ResetBusResponse&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:sro.ResetBusResponse)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_message().empty()) {
    _this->_internal_set_message(from._internal_message());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void ResetBusResponse::CopyFrom(const ResetBusResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:sro.ResetBusResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ResetBusResponse::IsInitialized() const {
  return true;
}

void ResetBusResponse::InternalSwap(ResetBusResponse* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.message_, lhs_arena,
                                       &other->_impl_.message_, rhs_arena);
}

::PROTOBUF_NAMESPACE_ID::Metadata ResetBusResponse::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_sro_2fbus_2eproto_getter, &descriptor_table_sro_2fbus_2eproto_once,
      file_level_metadata_sro_2fbus_2eproto[1]);
}
// @@protoc_insertion_point(namespace_scope)
}  // namespace sro
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::sro::BusTarget*
Arena::CreateMaybeMessage< ::sro::BusTarget >(Arena* arena) {
  return Arena::CreateMessageInternal< ::sro::BusTarget >(arena);
}
template<> PROTOBUF_NOINLINE ::sro::ResetBusResponse*
Arena::CreateMaybeMessage< ::sro::ResetBusResponse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::sro::ResetBusResponse >(arena);
}
PROTOBUF_NAMESPACE_CLOSE
// @@protoc_insertion_point(global_scope)
#include "google/protobuf/port_undef.inc"
