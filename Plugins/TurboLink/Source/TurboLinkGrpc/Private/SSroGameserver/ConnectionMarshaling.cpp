//Generated by TurboLink CodeGenerator, do not edit!
#include "ConnectionMarshaling.h"
#include "SGoogleApi/AnnotationsMarshaling.h"
#include "SSro/GlobalsMarshaling.h"

void GRPC_TO_TURBOLINK(const ::sro::gameserver::ConnectGameServerResponse* in, FGrpcSroGameserverConnectGameServerResponse* out)
{
    out->Address=UTF8_TO_TCHAR(in->address().c_str());
    out->Port=in->port();
    out->ConnectionId=UTF8_TO_TCHAR(in->connection_id().c_str());
}

void TURBOLINK_TO_GRPC(const FGrpcSroGameserverConnectGameServerResponse* in, ::sro::gameserver::ConnectGameServerResponse* out)
{
    out->set_address(TCHAR_TO_UTF8(*(in->Address)));
    out->set_port(in->Port);
    out->set_connection_id(TCHAR_TO_UTF8(*(in->ConnectionId)));
}

void GRPC_TO_TURBOLINK(const ::sro::gameserver::VerifyConnectRequest* in, FGrpcSroGameserverVerifyConnectRequest* out)
{
    out->ConnectionId=UTF8_TO_TCHAR(in->connection_id().c_str());
    out->ServerName=UTF8_TO_TCHAR(in->server_name().c_str());
}

void TURBOLINK_TO_GRPC(const FGrpcSroGameserverVerifyConnectRequest* in, ::sro::gameserver::VerifyConnectRequest* out)
{
    out->set_connection_id(TCHAR_TO_UTF8(*(in->ConnectionId)));
    out->set_server_name(TCHAR_TO_UTF8(*(in->ServerName)));
}

void GRPC_TO_TURBOLINK(const ::sro::gameserver::ConnectionStatus* in, FGrpcSroGameserverConnectionStatus* out)
{
    out->Online=in->online();
}

void TURBOLINK_TO_GRPC(const FGrpcSroGameserverConnectionStatus* in, ::sro::gameserver::ConnectionStatus* out)
{
    out->set_online(in->Online);
}

void GRPC_TO_TURBOLINK(const ::sro::gameserver::TransferPlayerRequest* in, FGrpcSroGameserverTransferPlayerRequest* out)
{
    out->Character=UTF8_TO_TCHAR(in->character().c_str());
    GRPC_TO_TURBOLINK(&(in->location()), &(out->Location));
}

void TURBOLINK_TO_GRPC(const FGrpcSroGameserverTransferPlayerRequest* in, ::sro::gameserver::TransferPlayerRequest* out)
{
    out->set_character(TCHAR_TO_UTF8(*(in->Character)));
    TURBOLINK_TO_GRPC(&(in->Location), out->mutable_location());
}

