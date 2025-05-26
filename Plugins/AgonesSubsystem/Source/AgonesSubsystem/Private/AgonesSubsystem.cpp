// Copyright Shattered Realms Online 2024


#include "AgonesSubsystem.h"

#include "AgonesModule.h"
#include "DataTypes.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"

template <typename CharType = TCHAR, typename PrintPolicy = TCondensedJsonPrintPolicy<TCHAR>>
bool JsonObjectToJsonString(const TSharedRef<FJsonObject>& JsonObject, FString& OutJson, int32 Indent = 0)
{
    TSharedRef<TJsonWriter<CharType, PrintPolicy>> JsonWriter = TJsonWriterFactory<CharType, PrintPolicy>::Create(&OutJson, Indent);
	bool bSuccess = FJsonSerializer::Serialize(JsonObject, JsonWriter);
	JsonWriter->Close();
	return bSuccess;
}

void UAgonesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	StartHealthPings(HealthRateSeconds);
	
	if (!bDisableAutoConnect)
	{
		Connect();
	}
}

void UAgonesSubsystem::Deinitialize()
{
	Super::Deinitialize();
	const UWorld* World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().ClearTimer(HealthTimerHandle);
		World->GetTimerManager().ClearTimer(ConnectTimerHandle);
		World->GetTimerManager().ClearTimer(WebSocketTimerHandle);
	}
}

bool UAgonesSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		// Only create an instance if there is no override implementation defined elsewhere
		return ChildClasses.Num() == 0;
	}

	return false;
}

void UAgonesSubsystem::StartHealthPings(float RateSeconds)
{
	if (RateSeconds <= 0.0f)
	{
		return;
	}

	FTimerDelegate TimerDel;
	FAgonesErrorDelegate ErrorDelegate;
	ErrorDelegate.BindUFunction(this, FName("HealthPingFailed"));
	TimerDel.BindUObject(this, &UAgonesSubsystem::Health, FHealthDelegate(), ErrorDelegate);
	GetWorld()->GetTimerManager().ClearTimer(HealthTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HealthTimerHandle, TimerDel, RateSeconds, true);
}

void UAgonesSubsystem::Connect()
{
	FGameServerDelegate SuccessDel;
	SuccessDel.BindUFunction(this, FName("ConnectSuccess"));
	FTimerDelegate ConnectDel;
	ConnectDel.BindUObject(this, &UAgonesSubsystem::GameServer, SuccessDel, FAgonesErrorDelegate());
	GetWorld()->GetTimerManager().ClearTimer(ConnectTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(ConnectTimerHandle, ConnectDel, 5.f, true);
}

void UAgonesSubsystem::ConnectSuccess(FGameServer GameServerResponse)
{
	GetWorld()->GetTimerManager().ClearTimer(ConnectTimerHandle);
	Ready({}, {});
	ConnectedDelegate.Broadcast(GameServerResponse);
}

void UAgonesSubsystem::GetListInternal(const FString& Key, FGetListInternalDelegate CompleteDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}"), {Key}), FHttpVerb::Get, "");
	Request->OnProcessRequestComplete().BindLambda(
		[CompleteDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			
			const FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				CompleteDelegate.ExecuteIfBound(FList{}, Error);
				return;
			}
			
			FList Response(JsonObject);
			CompleteDelegate.ExecuteIfBound(Response, TEXT(""));
		});
	Request->ProcessRequest();

}

void UAgonesSubsystem::Allocate(FAllocateDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest("allocate");
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}
	
			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::GameServer(FGameServerDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest("gameserver", FHttpVerb::Get, "");
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}
			
			FGameServer GameServer(JsonObject);
			SuccessDelegate.ExecuteIfBound(GameServer);
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::WatchGameServer(FGameServerDelegate WatchDelegate)
{
}

void UAgonesSubsystem::Health(FHealthDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest("health");
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}
			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::Ready(FReadyDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest("ready");
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::Reserve(int64 Seconds, FReserveDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	const FDuration Duration(Seconds);
	FString StringContent;
	if (!FJsonObjectConverter::UStructToJsonObjectString(Duration, StringContent))
	{
		ErrorDelegate.ExecuteIfBound(FAgonesError("unable to serialize request"));
		return;
	}
	
	FHttpRequestRef Request = BuildAgonesRequest("reserve", FHttpVerb::Post, StringContent);
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::SetAnnotation(const FString& Key, const FString& Value, FSetAnnotationDelegate SuccessDelegate,
	FAgonesErrorDelegate ErrorDelegate)
{
	const FKVPair Annotation(Key, Value);
	FString StringContent;
	if (!FJsonObjectConverter::UStructToJsonObjectString(Annotation, StringContent))
	{
		ErrorDelegate.ExecuteIfBound(FAgonesError("unable to serialize request"));
		return;
	}
	
	FHttpRequestRef Request = BuildAgonesRequest("metadata/annotation", FHttpVerb::Put, StringContent);
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::SetLabel(const FString& Key, const FString& Value, FSetLabelDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	const FKVPair Label(Key, Value);
	FString StringContent;
	if (!FJsonObjectConverter::UStructToJsonObjectString(Label, StringContent))
	{
		ErrorDelegate.ExecuteIfBound(FAgonesError("unable to serialize request"));
		return;
	}
	
	FHttpRequestRef Request = BuildAgonesRequest("metadata/label", FHttpVerb::Put, StringContent);
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::Shutdown(FShutdownDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest("shutdown");
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::GetConnectedPlayers(FGetConnectedPlayersDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	const auto Delegate = FGetListInternalDelegate::CreateLambda([SuccessDelegate, ErrorDelegate](FList Response, FString Error) {
		if (!Error.IsEmpty())
		{
			ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
			return;
		}

		SuccessDelegate.ExecuteIfBound(FConnectedPlayers(Response));
	});
	GetListInternal(PLAYERS_KEY, Delegate);
}

void UAgonesSubsystem::GetPlayerCapacity(FGetPlayerCapacityDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	const auto Delegate = FGetListInternalDelegate::CreateLambda([SuccessDelegate, ErrorDelegate](FList Response, FString Error) {
		if (!Error.IsEmpty())
		{
			ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
			return;
		}

		SuccessDelegate.ExecuteIfBound(FCount(Response.Capacity));
	});
	GetListInternal(PLAYERS_KEY, Delegate);
}

void UAgonesSubsystem::GetPlayerCount(FGetPlayerCountDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	const auto Delegate = FGetListInternalDelegate::CreateLambda([SuccessDelegate, ErrorDelegate](FList Response, FString Error) {
		if (!Error.IsEmpty())
		{
			ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
			return;
		}

		SuccessDelegate.ExecuteIfBound(FCount(Response.List.Num()));
	});
	GetListInternal(PLAYERS_KEY, Delegate);
}

void UAgonesSubsystem::IsPlayerConnected(FString PlayerId, FIsPlayerConnectedDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	const auto Delegate = FGetListInternalDelegate::CreateLambda([PlayerId, SuccessDelegate, ErrorDelegate](FList Response, FString Error) {
		if (!Error.IsEmpty())
		{
			ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
			return;
		}

		for (const auto& CurrentPlayerId : Response.List)
		{
			if (CurrentPlayerId == PlayerId)
			{
				SuccessDelegate.ExecuteIfBound(FBool(true));
				return;
			}
		}
		SuccessDelegate.ExecuteIfBound(FBool(false));
	});
	GetListInternal(PLAYERS_KEY, Delegate);
}

void UAgonesSubsystem::PlayerConnect(FString PlayerId, FPlayerConnectDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("value"), PlayerId);
	
	FString JsonContent;
	if (!JsonObjectToJsonString(JsonObject, JsonContent))
	{
		ErrorDelegate.ExecuteIfBound(FAgonesError("Failed to serialize request"));
		return;
	}

	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}:addValue"), {PLAYERS_KEY}), FHttpVerb::Post, JsonContent);
	Request->OnProcessRequestComplete().BindLambda(
		[PlayerId, SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound(FBool(true));
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::PlayerDisconnect(FString PlayerId, FPlayerDisconnectDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("value"), PlayerId);
	
	FString JsonContent;
	if (!JsonObjectToJsonString(JsonObject, JsonContent))
	{
		ErrorDelegate.ExecuteIfBound(FAgonesError("Failed to serialize request"));
		return;
	}

	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}:removeValue"), {PLAYERS_KEY}), FHttpVerb::Post, JsonContent);
	Request->OnProcessRequestComplete().BindLambda(
		[PlayerId, SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound(FBool(true));
		});
	Request->ProcessRequest();
}

void UAgonesSubsystem::SetPlayerCapacity(int64 Count, FSetPlayerCapacityDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate)
{
	TSharedRef<FJsonObject> ListJsonObject = MakeShareable(new FJsonObject());
	ListJsonObject->SetNumberField(TEXT("capacity"), Count);
	ListJsonObject->SetStringField(TEXT("name"), PLAYERS_KEY);

	TSharedRef<FJsonObject> MaskJsonObject = MakeShareable(new FJsonObject());
	TSharedPtr<FJsonValue> MaskValue = MakeShareable(new FJsonValueString("capacity"));
	MaskJsonObject->SetArrayField(TEXT("paths"), {MaskValue});
	
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetObjectField(TEXT("list"), ListJsonObject);
	JsonObject->SetObjectField(TEXT("update_mask"), MaskJsonObject);
	
	FString JsonContent;
	if (!JsonObjectToJsonString(JsonObject, JsonContent))
	{
		ErrorDelegate.ExecuteIfBound(FAgonesError("Failed to serialize request"));
		return;
	}

	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}"), {PLAYERS_KEY}), FHttpVerb::Patch, JsonContent);
	Request->OnProcessRequestComplete().BindLambda(
		[SuccessDelegate, ErrorDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				ErrorDelegate.ExecuteIfBound(FAgonesError(Error));
				return;
			}

			SuccessDelegate.ExecuteIfBound();
		});
	Request->ProcessRequest();
}

FHttpRequestRef UAgonesSubsystem::BuildAgonesRequest(const FString& Path, const FHttpVerb Verb, const FString& Content)
{
	FHttpModule* Http = &FHttpModule::Get();
	FHttpRequestRef Request = Http->CreateRequest();

	Request->SetURL(FString::Format(
		TEXT("http://localhost:{0}/{1}"), 
		{FStringFormatArg(HttpPort), FStringFormatArg(Path)}	
	));
	Request->SetVerb(Verb.ToString());
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
	Request->SetContentAsString(Content);
	return Request;
}

FString UAgonesSubsystem::IsValidResponse(const bool bSucceeded, const FHttpResponsePtr HttpResponse)
{
	if (!bSucceeded)
	{
		return FString("Unsuccessful");
	}

	if (!EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
	{
		return FString::Format(TEXT("Error {0}"),
			static_cast<FStringFormatOrderedArguments>(
				TArray<FStringFormatArg, TFixedAllocator<1>>{
					FStringFormatArg(FString::FromInt(HttpResponse->GetResponseCode()))
				})
			);
	}

	return FString();
}

FString UAgonesSubsystem::IsValidJsonResponse(TSharedPtr<FJsonObject>& JsonObject, const bool bSucceeded, const FHttpResponsePtr HttpResponse)
{
	FString Error = IsValidResponse(bSucceeded, HttpResponse);
	if (!Error.IsEmpty())
	{
		return Error;
	}

	TSharedPtr<FJsonObject> OutObject;
	const FString Json = HttpResponse->GetContentAsString();
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Json);
	if (!FJsonSerializer::Deserialize(JsonReader, OutObject) || !OutObject.IsValid())
	{
		return FString::Format(TEXT("Failed to parse response - {0}"),
			static_cast<FStringFormatOrderedArguments>(
				TArray<FStringFormatArg, TFixedAllocator<1>>{
					FStringFormatArg(Json)
                })
            );
	}

	JsonObject = OutObject.ToSharedRef();
	return FString(); 
}

void UAgonesSubsystem::HealthPingFailed(FAgonesError& Error)
{
	UE_LOG(LogAgones, Warning, TEXT("Health ping failed."));
}
