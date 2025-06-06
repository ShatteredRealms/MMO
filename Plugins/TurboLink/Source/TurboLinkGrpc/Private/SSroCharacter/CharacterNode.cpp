//Generated by TurboLink CodeGenerator, do not edit!
#include "SSroCharacter/CharacterNode.h"
#include "SSroCharacter/CharacterService.h"
#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcUtilities.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Runtime/Launch/Resources/Version.h"

UCallCharacterServiceGetCharacter* UCallCharacterServiceGetCharacter::GetCharacter(UObject* WorldContextObject, const FGrpcSroCharacterGetCharacterRequest& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceGetCharacter* node = NewObject<UCallCharacterServiceGetCharacter>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceGetCharacter::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceGetCharacter::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceGetCharacter::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcSroCharacterCharacter response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceGetCharacter::OnContextStateChange);
		CharacterServiceClient->OnGetCharacterResponse.AddUniqueDynamic(this, &UCallCharacterServiceGetCharacter::OnResponse);

		Context = CharacterServiceClient->InitGetCharacter();
		CharacterServiceClient->GetCharacter(Context, Request, MetaData);
	}
}

void UCallCharacterServiceGetCharacter::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceGetCharacter::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacter& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnGetCharacterResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceGetCharacter::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceGetCharacter::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

UCallCharacterServiceGetCharacters* UCallCharacterServiceGetCharacters::GetCharacters(UObject* WorldContextObject, const FGrpcSroCharacterGetCharactersRequest& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceGetCharacters* node = NewObject<UCallCharacterServiceGetCharacters>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceGetCharacters::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceGetCharacters::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceGetCharacters::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcSroCharacterCharacters response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceGetCharacters::OnContextStateChange);
		CharacterServiceClient->OnGetCharactersResponse.AddUniqueDynamic(this, &UCallCharacterServiceGetCharacters::OnResponse);

		Context = CharacterServiceClient->InitGetCharacters();
		CharacterServiceClient->GetCharacters(Context, Request, MetaData);
	}
}

void UCallCharacterServiceGetCharacters::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceGetCharacters::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacters& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnGetCharactersResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceGetCharacters::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceGetCharacters::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

UCallCharacterServiceGetCharactersForUser* UCallCharacterServiceGetCharactersForUser::GetCharactersForUser(UObject* WorldContextObject, const FGrpcSroCharacterGetUserCharactersRequest& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceGetCharactersForUser* node = NewObject<UCallCharacterServiceGetCharactersForUser>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceGetCharactersForUser::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceGetCharactersForUser::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceGetCharactersForUser::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcSroCharacterCharacters response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceGetCharactersForUser::OnContextStateChange);
		CharacterServiceClient->OnGetCharactersForUserResponse.AddUniqueDynamic(this, &UCallCharacterServiceGetCharactersForUser::OnResponse);

		Context = CharacterServiceClient->InitGetCharactersForUser();
		CharacterServiceClient->GetCharactersForUser(Context, Request, MetaData);
	}
}

void UCallCharacterServiceGetCharactersForUser::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceGetCharactersForUser::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacters& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnGetCharactersForUserResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceGetCharactersForUser::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceGetCharactersForUser::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

UCallCharacterServiceCreateCharacter* UCallCharacterServiceCreateCharacter::CreateCharacter(UObject* WorldContextObject, const FGrpcSroCharacterCreateCharacterRequest& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceCreateCharacter* node = NewObject<UCallCharacterServiceCreateCharacter>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceCreateCharacter::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceCreateCharacter::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceCreateCharacter::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcSroCharacterCharacter response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceCreateCharacter::OnContextStateChange);
		CharacterServiceClient->OnCreateCharacterResponse.AddUniqueDynamic(this, &UCallCharacterServiceCreateCharacter::OnResponse);

		Context = CharacterServiceClient->InitCreateCharacter();
		CharacterServiceClient->CreateCharacter(Context, Request, MetaData);
	}
}

void UCallCharacterServiceCreateCharacter::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceCreateCharacter::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacter& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnCreateCharacterResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceCreateCharacter::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceCreateCharacter::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

UCallCharacterServiceDeleteCharacter* UCallCharacterServiceDeleteCharacter::DeleteCharacter(UObject* WorldContextObject, const FGrpcSroTargetId& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceDeleteCharacter* node = NewObject<UCallCharacterServiceDeleteCharacter>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceDeleteCharacter::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceDeleteCharacter::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceDeleteCharacter::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcGoogleProtobufEmpty response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceDeleteCharacter::OnContextStateChange);
		CharacterServiceClient->OnDeleteCharacterResponse.AddUniqueDynamic(this, &UCallCharacterServiceDeleteCharacter::OnResponse);

		Context = CharacterServiceClient->InitDeleteCharacter();
		CharacterServiceClient->DeleteCharacter(Context, Request, MetaData);
	}
}

void UCallCharacterServiceDeleteCharacter::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceDeleteCharacter::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnDeleteCharacterResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceDeleteCharacter::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceDeleteCharacter::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

UCallCharacterServiceEditCharacter* UCallCharacterServiceEditCharacter::EditCharacter(UObject* WorldContextObject, const FGrpcSroCharacterEditCharacterRequest& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceEditCharacter* node = NewObject<UCallCharacterServiceEditCharacter>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceEditCharacter::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceEditCharacter::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceEditCharacter::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcSroCharacterCharacter response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceEditCharacter::OnContextStateChange);
		CharacterServiceClient->OnEditCharacterResponse.AddUniqueDynamic(this, &UCallCharacterServiceEditCharacter::OnResponse);

		Context = CharacterServiceClient->InitEditCharacter();
		CharacterServiceClient->EditCharacter(Context, Request, MetaData);
	}
}

void UCallCharacterServiceEditCharacter::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceEditCharacter::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacter& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnEditCharacterResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceEditCharacter::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceEditCharacter::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}

UCallCharacterServiceAddCharacterPlayTime* UCallCharacterServiceAddCharacterPlayTime::AddCharacterPlayTime(UObject* WorldContextObject, const FGrpcSroCharacterAddPlayTimeRequest& Request, TMap<FString, FString>& MetaData)
{
	UCallCharacterServiceAddCharacterPlayTime* node = NewObject<UCallCharacterServiceAddCharacterPlayTime>(WorldContextObject);
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(WorldContextObject);

	node->CharacterService = Cast<UCharacterService>(turboLinkManager->MakeService("CharacterService"));
	if (node->CharacterService == nullptr)
	{
		return nullptr;
	}
	node->Request = Request;
	node->ServiceState = EGrpcServiceState::Idle;
	node->MetaData = MetaData;

	node->CharacterService->OnServiceStateChanged.AddUniqueDynamic(node, &UCallCharacterServiceAddCharacterPlayTime::OnServiceStateChanged);
	return node;
}

void UCallCharacterServiceAddCharacterPlayTime::Activate()
{
	CharacterService->Connect();
}

void UCallCharacterServiceAddCharacterPlayTime::OnServiceStateChanged(EGrpcServiceState NewState)
{
	if (ServiceState == NewState) return;
	ServiceState = NewState;

	if (NewState == EGrpcServiceState::TransientFailure)
	{
		FGrpcResult result;
		result.Code = EGrpcResultCode::ConnectionFailed;

		FGrpcGoogleProtobufEmpty response;
		OnFail.Broadcast(result, response);

		Shutdown();
		return;
	}

	if (NewState == EGrpcServiceState::Ready)
	{
		CharacterServiceClient = CharacterService->MakeClient();
		CharacterServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UCallCharacterServiceAddCharacterPlayTime::OnContextStateChange);
		CharacterServiceClient->OnAddCharacterPlayTimeResponse.AddUniqueDynamic(this, &UCallCharacterServiceAddCharacterPlayTime::OnResponse);

		Context = CharacterServiceClient->InitAddCharacterPlayTime();
		CharacterServiceClient->AddCharacterPlayTime(Context, Request, MetaData);
	}
}

void UCallCharacterServiceAddCharacterPlayTime::OnContextStateChange(FGrpcContextHandle Handle, EGrpcContextState State)
{
	if (State == EGrpcContextState::Done)
	{
		Shutdown();
	}
}

void UCallCharacterServiceAddCharacterPlayTime::OnResponse(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)
{
	if (GrpcResult.Code == EGrpcResultCode::Ok)
	{
		OnAddCharacterPlayTimeResponse.Broadcast(GrpcResult, Response);
	}
	else
	{
		OnFail.Broadcast(GrpcResult, Response);
	}
}

void UCallCharacterServiceAddCharacterPlayTime::Shutdown()
{
	CharacterService->OnServiceStateChanged.RemoveDynamic(this, &UCallCharacterServiceAddCharacterPlayTime::OnServiceStateChanged);
	if (CharacterServiceClient != nullptr)
	{
		CharacterService->RemoveClient(CharacterServiceClient);
		CharacterServiceClient->Shutdown();
		CharacterServiceClient = nullptr;
	}

	if (CharacterService != nullptr)
	{
		UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this)->ReleaseService(CharacterService);
		CharacterService = nullptr;
	}

	SetReadyToDestroy();
#if ENGINE_MAJOR_VERSION>=5
	MarkAsGarbage();
#else
	MarkPendingKill();
#endif
}
