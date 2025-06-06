// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: sro/character/inventory.proto

#include "sro/character/inventory.pb.h"
#include "sro/character/inventory.grpc.pb.h"

#include <functional>
#include <grpcpp/support/async_stream.h>
#include <grpcpp/support/async_unary_call.h>
#include <grpcpp/impl/channel_interface.h>
#include <grpcpp/impl/client_unary_call.h>
#include <grpcpp/support/client_callback.h>
#include <grpcpp/support/message_allocator.h>
#include <grpcpp/support/method_handler.h>
#include <grpcpp/impl/rpc_service_method.h>
#include <grpcpp/support/server_callback.h>
#include <grpcpp/impl/server_callback_handlers.h>
#include <grpcpp/server_context.h>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/sync_stream.h>
namespace sro {
namespace character {

static const char* InventoryService_method_names[] = {
  "/sro.character.InventoryService/GetInventory",
};

std::unique_ptr< InventoryService::Stub> InventoryService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  (void)options;
  std::unique_ptr< InventoryService::Stub> stub(new InventoryService::Stub(channel, options));
  return stub;
}

InventoryService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options)
  : channel_(channel), rpcmethod_GetInventory_(InventoryService_method_names[0], options.suffix_for_stats(),::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status InventoryService::Stub::GetInventory(::grpc::ClientContext* context, const ::sro::character::GetInventoryRequest& request, ::sro::character::GetInventoryResponse* response) {
  return ::grpc::internal::BlockingUnaryCall< ::sro::character::GetInventoryRequest, ::sro::character::GetInventoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), rpcmethod_GetInventory_, context, request, response);
}

void InventoryService::Stub::async::GetInventory(::grpc::ClientContext* context, const ::sro::character::GetInventoryRequest* request, ::sro::character::GetInventoryResponse* response, std::function<void(::grpc::Status)> f) {
  ::grpc::internal::CallbackUnaryCall< ::sro::character::GetInventoryRequest, ::sro::character::GetInventoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetInventory_, context, request, response, std::move(f));
}

void InventoryService::Stub::async::GetInventory(::grpc::ClientContext* context, const ::sro::character::GetInventoryRequest* request, ::sro::character::GetInventoryResponse* response, ::grpc::ClientUnaryReactor* reactor) {
  ::grpc::internal::ClientCallbackUnaryFactory::Create< ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(stub_->channel_.get(), stub_->rpcmethod_GetInventory_, context, request, response, reactor);
}

::grpc::ClientAsyncResponseReader< ::sro::character::GetInventoryResponse>* InventoryService::Stub::PrepareAsyncGetInventoryRaw(::grpc::ClientContext* context, const ::sro::character::GetInventoryRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::sro::character::GetInventoryResponse, ::sro::character::GetInventoryRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetInventory_, context, request);
}

::grpc::ClientAsyncResponseReader< ::sro::character::GetInventoryResponse>* InventoryService::Stub::AsyncGetInventoryRaw(::grpc::ClientContext* context, const ::sro::character::GetInventoryRequest& request, ::grpc::CompletionQueue* cq) {
  auto* result =
    this->PrepareAsyncGetInventoryRaw(context, request, cq);
  result->StartCall();
  return result;
}

InventoryService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      InventoryService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< InventoryService::Service, ::sro::character::GetInventoryRequest, ::sro::character::GetInventoryResponse, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(
          [](InventoryService::Service* service,
             ::grpc::ServerContext* ctx,
             const ::sro::character::GetInventoryRequest* req,
             ::sro::character::GetInventoryResponse* resp) {
               return service->GetInventory(ctx, req, resp);
             }, this)));
}

InventoryService::Service::~Service() {
}

::grpc::Status InventoryService::Service::GetInventory(::grpc::ServerContext* context, const ::sro::character::GetInventoryRequest* request, ::sro::character::GetInventoryResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace sro
}  // namespace character

