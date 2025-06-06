//Generated by TurboLink CodeGenerator, do not edit!
#include "SSroGameserver/DataNode.h"
#include "SSroGameserver/DataService.h"
#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcUtilities.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Runtime/Launch/Resources/Version.h"

UCallGameServerDataServiceGetGameServerDetails* UCallGameServerDataServiceGetGameServerDetails::GetGameServerDetails(UObject* WorldContextObject, const FGrpcGoogleProtobufEmpty& Request, TMap<FString, FString>& MetaData)
{
	UCallGameServerDataServiceGetGameServerDetails* node = NewObject<UCallGameServerDataServiceGetGameServerDetails>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->GameServerDataService = Cast<UGameServerDataService>(turboLinkManager->MakeService("GameServerDataService"));
	if (node->GameServerDataService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->GameServerDataService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallGameServerDataServiceGetGameServerDetails::OnServiceStateChanged);
	return node;
}

void UCallGameServerDataServiceGetGameServerDetails::Activate()
{
	GameServerDataService->Connect();
}

void UCallGameServerDataServiceGetGameServerDetails::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcSroGameserverGameServerDetails response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		GameServerDataServiceClient = GameServerDataService->MakeClient();
		GameServerDataServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallGameServerDataServiceGetGameServerDetails::OnContextStateChange);
		GameServerDataServiceClient->OnGetGameServerDetailsResponse.AddUniqueDynamic(this, &UCallGameServerDataServiceGetGameServerDetails::OnResponse);

		Context = GameServerDataServiceClient->InitGetGameServerDetails();
		GameServerDataServiceClient->GetGameServerDetails(Context, Request, MetaData);
	}
}

void UCallGameServerDataServiceGetGameServerDetails::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallGameServerDataServiceGetGameServerDetails::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroGameserverGameServerDetails& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnGetGameServerDetailsResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallGameServerDataServiceGetGameServerDetails::Shutdown()
{
	GameServerDataService->OnServiceStateChanged.RemoveDynamic(this, &UCallGameServerDataServiceGetGameServerDetails::OnServiceStateChanged);
	if (GameServerDataServiceClient != nullptr)
	{
		GameServerDataService->RemoveClient(GameServerDataServiceClient);
		GameServerDataServiceClient->Shutdown();
		GameServerDataServiceClient = nullptr;
	}

	if (GameServerDataService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(GameServerDataService);
		GameServerDataService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
