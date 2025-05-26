#include "OnlineSessionSRO.h"

#include "HttpModule.h"
#include "OnlineIdentitySRO.h"
#include "OnlineSubsystemSRO.h"
#include "Interfaces/IHttpResponse.h"

template <typename CharType = TCHAR, typename PrintPolicy = TCondensedJsonPrintPolicy<TCHAR>>
bool JsonObjectToJsonString(const TSharedRef<FJsonObject>& JsonObject, FString& OutJson, int32 Indent = 0)
{
    TSharedRef<TJsonWriter<CharType, PrintPolicy>> JsonWriter = TJsonWriterFactory<CharType, PrintPolicy>::Create(&OutJson, Indent);
	bool bSuccess = FJsonSerializer::Serialize(JsonObject, JsonWriter);
	JsonWriter->Close();
	return bSuccess;
}

FOnlineSessionSRO::~FOnlineSessionSRO()
{
	if (IsRunningDedicatedServer())
	{
		EndSession(NAME_GameSession);
	}
}

FOnlineSessionSRO::FOnlineSessionSRO(FOnlineSubsystemSRO* InSubsystem)
	: CurrentSessionSearch(nullptr)
	, SessionSearchStartInSeconds(0)
	, SROSubsystem(InSubsystem)
{
#if WITH_SERVER_CODE
	if (IsRunningDedicatedServer())
	{
		if (!GConfig->GetString(INI_SECTION, AGONES_PORT, AgonesPort, GEngineIni))
		{
			UE_LOG_ONLINE_SESSION(Log, TEXT("Missing %s= in [%s] of DefaultEngine.ini, using default value [%s]"), *FString(AGONES_PORT), *FString(INI_SECTION), *AgonesPort);
		}
		if (!GConfig->GetFloat(INI_SECTION, HEALTH_RATE, HealthRate, GEngineIni))
		{
			UE_LOG_ONLINE_SESSION(Log, TEXT("Missing %s= in [%s] of DefaultEngine.ini, using default value [%f]"), *FString(HEALTH_RATE), *FString(INI_SECTION), HealthRate);
		}
		else
		{
			HealthTimer = -HealthRate;
		}

		UE_LOG_ONLINE_SESSION(Log, TEXT("Calling connect"));
		Connect();
		UE_LOG_ONLINE_SESSION(Log, TEXT("Calling HealthCheck"));
		HealthCheck();
	}
#else
		UE_LOG_ONLINE_SESSION(Log, TEXT("NOT WITH SERVER CODE"));
#endif
}

FUniqueNetIdPtr FOnlineSessionSRO::CreateSessionIdFromString(const FString& SessionIdStr)
{
	return FUniqueNetIdSROSession::Create(SessionIdStr);
}

bool FOnlineSessionSRO::CreateSession(int32 HostingPlayerNum, FName SessionName,
                                      const FOnlineSessionSettings& NewSessionSettings)
{
	FScopeLock ScopeLock(&SessionLock);
	FOnlineSessionSettings Settings = NewSessionSettings;
	Settings.bAllowInvites = false;
	Settings.bIsDedicated = true;
	Settings.bShouldAdvertise = false;
	Settings.bUsesPresence = false;
	Settings.bUsesStats = false;
	Settings.bAntiCheatProtected = false;
	Settings.bAllowJoinInProgress = true;
	Settings.bAllowJoinViaPresence = false;
	Settings.bUseLobbiesIfAvailable = false;
	Settings.bIsLANMatch = false;
	Settings.bAllowJoinViaPresenceFriendsOnly = false;
	Settings.bUseLobbiesVoiceChatIfAvailable = false;

	FName ActualSessionName = SessionName;
	if (CachedGameServer.IsValid())
	{
		ActualSessionName = FName(*CachedGameServer.ObjectMeta.Name);
	}
	
	NamedOnlineSession = new FNamedOnlineSession(ActualSessionName, Settings);
	return true;
}

bool FOnlineSessionSRO::CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName,
	const FOnlineSessionSettings& NewSessionSettings)
{
	return CreateSession(SROSubsystem->SROIdentity->GetLocalUserNumFromUniqueNetId(HostingPlayerId), SessionName, NewSessionSettings);
}

bool FOnlineSessionSRO::StartSession(FName SessionName)
{
	Connect();
	return true;
}

bool FOnlineSessionSRO::UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings,
	bool bShouldRefreshOnlineData)
{
	return CreateSession(0, SessionName, UpdatedSessionSettings);
}

bool FOnlineSessionSRO::EndSession(FName SessionName)
{
	UnregisterPlayers(FName(), ConnectedPlayers.Array());
	Shutdown();
	return false;
}

bool FOnlineSessionSRO::DestroySession(FName SessionName, const FOnDestroySessionCompleteDelegate& CompletionDelegate)
{
	EndSession(SessionName);
	return false;
}

bool FOnlineSessionSRO::IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId)
{
	return ConnectedPlayers.Contains(UniqueId.AsShared());
}

bool FOnlineSessionSRO::StartMatchmaking(const TArray<FUniqueNetIdRef>& LocalPlayers, FName SessionName,
	const FOnlineSessionSettings& NewSessionSettings, TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionSRO::CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName)
{
	return false;
}

bool FOnlineSessionSRO::CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName)
{
	return false;
}

bool FOnlineSessionSRO::FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionSRO::FindSessions(const FUniqueNetId& SearchingPlayerId,
	const TSharedRef<FOnlineSessionSearch>& SearchSettings)
{
	return false;
}

bool FOnlineSessionSRO::FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId,
	const FUniqueNetId& FriendId, const FOnSingleSessionResultCompleteDelegate& CompletionDelegate)
{
	return false;
}

bool FOnlineSessionSRO::CancelFindSessions()
{
	return false;
}

bool FOnlineSessionSRO::PingSearchResults(const FOnlineSessionSearchResult& SearchResult)
{
	return false;
}

bool FOnlineSessionSRO::JoinSession(int32 PlayerNum, FName SessionName,
	const FOnlineSessionSearchResult& DesiredSession)
{
	return false;
}

bool FOnlineSessionSRO::JoinSession(const FUniqueNetId& PlayerId, FName SessionName,
	const FOnlineSessionSearchResult& DesiredSession)
{
	return JoinSession(SROSubsystem->SROIdentity->GetLocalUserNumFromUniqueNetId(PlayerId), SessionName, DesiredSession);
}

bool FOnlineSessionSRO::FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionSRO::FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionSRO::FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<FUniqueNetIdRef>& FriendList)
{
	return false;
}

bool FOnlineSessionSRO::SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionSRO::SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName,
	const FUniqueNetId& Friend)
{
	return false;
}

bool FOnlineSessionSRO::SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName,
	const TArray<FUniqueNetIdRef>& Friends)
{
	return false;
}

bool FOnlineSessionSRO::SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName,
	const TArray<FUniqueNetIdRef>& Friends)
{
	return false;
}

bool FOnlineSessionSRO::GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType)
{
	return false;
}

bool FOnlineSessionSRO::GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType,
	FString& ConnectInfo)
{
	return false;
}

FOnlineSessionSettings* FOnlineSessionSRO::GetSessionSettings(FName SessionName)
{
	return nullptr;
}

FString FOnlineSessionSRO::GetVoiceChatRoomName(int32 LocalUserNum, const FName& SessionName)
{
	return IOnlineSession::GetVoiceChatRoomName(LocalUserNum, SessionName);
}

bool FOnlineSessionSRO::RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited)
{
	return RegisterPlayers(SessionName, {PlayerId.AsShared()}, bWasInvited);
}

bool FOnlineSessionSRO::RegisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players, bool bWasInvited)
{
	UE_LOG_ONLINE_SESSION(Log, TEXT("Calling RegisterPlayers"));

	if (!bIsAgonesConnected)
	{
		return false;
	}
	
	for (const FUniqueNetIdRef& PlayerId : Players)
	{
		UE_LOG_ONLINE_SESSION(Log, TEXT("Registering player %s"), *PlayerId->ToString());
		PlayerConnect(PlayerId->ToString());
		ConnectedPlayers.Add(PlayerId);
	}
	return true;
}

bool FOnlineSessionSRO::UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId)
{
	return UnregisterPlayers(SessionName, {PlayerId.AsShared()});
}

bool FOnlineSessionSRO::UnregisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players)
{
	if (!bIsAgonesConnected)
	{
		return false;
	}
	
	for (const FUniqueNetIdRef& PlayerId : Players)
	{
		PlayerDisconnect(PlayerId->ToString());
		ConnectedPlayers.Remove(PlayerId);
	}
	return true;
}

void FOnlineSessionSRO::RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName,
	const FOnRegisterLocalPlayerCompleteDelegate& Delegate)
{
	RegisterPlayers(SessionName, {PlayerId.AsShared()}, false);
}

void FOnlineSessionSRO::UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName,
	const FOnUnregisterLocalPlayerCompleteDelegate& Delegate)
{
	UnregisterPlayers(SessionName, {PlayerId.AsShared()});
}

void FOnlineSessionSRO::RemovePlayerFromSession(int32 LocalUserNum, FName SessionName,
	const FUniqueNetId& TargetPlayerId)
{
}

int32 FOnlineSessionSRO::GetNumSessions()
{
#if WITH_SERVER_CODE
	return bIsAgonesConnected ? 1 : 0;
#else
	return 0;
#endif
}

void FOnlineSessionSRO::DumpSessionState()
{
}

void FOnlineSessionSRO::Tick(float DeltaTime)
{
#if WITH_SERVER_CODE
	if (HealthRate > 0)
	{
		HealthTimer += DeltaTime;
		if (HealthTimer > HealthRate)
		{
			HealthTimer = 0.0f;
			HealthCheck();
		}
	}

	if (GameServerRate > 0)
	{
		GameServerTimer += DeltaTime;
		if (GameServerTimer > GameServerRate)
		{
			GameServerTimer = 0.0f;
			GameServer();
		}
	}
#endif
}

void FOnlineSessionSRO::HealthCheck()
{
#if WITH_SERVER_CODE
	FHttpRequestRef Request = BuildAgonesRequest("health");
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::SetLabel(const FString& Key, const FString& Value)
{
#if WITH_SERVER_CODE
	const FKVPair Label(Key, Value);
	FHttpRequestRef Request = BuildAgonesRequest("metadata/label", FHttpVerb::Put, Label.ToJson(false));
	Request->OnProcessRequestComplete().BindLambda(
		[this, Key, Value](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to set label (%s, %s): %s"), *Key, *Value, *Error);
				return;
			}
	
			OnSetLabel.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::Health()
{
#if WITH_SERVER_CODE
	FHttpRequestRef Request = BuildAgonesRequest("health");
	Request->OnProcessRequestComplete().BindLambda(
		[](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed health ping: %s"), *Error);
			}
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::Shutdown()
{
#if WITH_SERVER_CODE
	FHttpRequestRef Request = BuildAgonesRequest("shutdown");
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to shutdown: %s"), *Error);
				return;
			}
	
			OnShutdown.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::SetAnnotation(const FString& Key, const FString& Value)
{
#if WITH_SERVER_CODE
	const FKVPair Annotation(Key, Value);
	FHttpRequestRef Request = BuildAgonesRequest("metadata/annotation", FHttpVerb::Put, Annotation.ToJson(false));
	Request->OnProcessRequestComplete().BindLambda(
		[this, Key, Value](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to set annotation (%s, %s): %s"), *Key, *Value, *Error);
				return;
			}
	
			OnSetAnnotation.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::Allocate()
{
#if WITH_SERVER_CODE
	FHttpRequestRef Request = BuildAgonesRequest("allocate");
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to shutdown: %s"), *Error);
				return;
			}
	
			OnAllocate.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::Reserve(const int64 Seconds)
{
#if WITH_SERVER_CODE
	const FDuration Duration(Seconds);
	FHttpRequestRef Request = BuildAgonesRequest("reserve", FHttpVerb::Post, Duration.ToJson(false));
	Request->OnProcessRequestComplete().BindLambda(
		[this, Seconds](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to reserve (%llds): %s"), Seconds, *Error);
				return;
			}
	
			OnReserve.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}


void FOnlineSessionSRO::PlayerConnect(const FString PlayerId)
{
#if WITH_SERVER_CODE
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("value"), PlayerId);
	
	FString JsonContent;
	if (!JsonObjectToJsonString(JsonObject, JsonContent))
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to connect player (%s): failed to serialize request"), *PlayerId);
		return;
	}

	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}:addValue"), {PLAYERS_KEY}), FHttpVerb::Post, JsonContent);
	Request->OnProcessRequestComplete().BindLambda(
		[this, PlayerId](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to connect player (%s): %s: %s"), *PlayerId, *Error, *HttpResponse->GetContentAsString());
				return;
			}

			OnPlayerConnect.ExecuteIfBound(FBool(true));
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::PlayerDisconnect(const FString PlayerId)
{
#if WITH_SERVER_CODE
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	JsonObject->SetStringField(TEXT("value"), PlayerId);
	
	FString JsonContent;
	if (!JsonObjectToJsonString(JsonObject, JsonContent))
	{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to disconnect player (%s): failed to serialize request"), *PlayerId);
		return;
	}

	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}:removeValue"), {PLAYERS_KEY}), FHttpVerb::Post, JsonContent);
	Request->OnProcessRequestComplete().BindLambda(
		[this, PlayerId](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to disconnect player (%s). Error %s: %s"), *PlayerId, *Error, *HttpResponse->GetContentAsString());
				return;
			}

			OnPlayerDisconnect.ExecuteIfBound(FBool(true));
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::SetPlayerCapacity(const int64 Count)
{
#if WITH_SERVER_CODE
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
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to set player capacity (%lld): failed to serialize request"), Count);
		return;
	}

	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}"), {PLAYERS_KEY}), FHttpVerb::Patch, JsonContent);
	Request->OnProcessRequestComplete().BindLambda(
		[this, Count](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
		UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to set player capacity (%lld): %s"), Count, *Error);
				return;
			}

			OnSetPlayerCapacity.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::GetPlayerCapacity()
{
#if WITH_SERVER_CODE
	const auto Delegate = FGetListInternalDelegate::CreateLambda([this](FListResponse Response, FString Error) {
		if (!Error.IsEmpty())
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to get connected players: %s"), *Error);
			return;
		}
		OnGetPlayerCapacity.ExecuteIfBound(FPlayerCount(Response.Capacity));
	});
	GetListInternal(PLAYERS_KEY, Delegate);
#endif
}

void FOnlineSessionSRO::GetPlayerCount()
{
#if WITH_SERVER_CODE
	const auto Delegate = FGetListInternalDelegate::CreateLambda([this](FListResponse Response, FString Error) {
		if (!Error.IsEmpty())
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to get connected players: %s"), *Error);
			return;
		}
		OnGetPlayerCount.ExecuteIfBound(FPlayerCount(Response.List.Num()));
	});
	GetListInternal(PLAYERS_KEY, Delegate);
#endif
}

void FOnlineSessionSRO::IsPlayerConnected(const FString PlayerId)
{
#if WITH_SERVER_CODE
	const auto Delegate = FGetListInternalDelegate::CreateLambda([this, PlayerId](FListResponse Response, FString Error) {
		if (!Error.IsEmpty())
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to get connected players: %s"), *Error);
			return;
		}
		for (const auto CurrentPlayerId : Response.List)
		{
			if (CurrentPlayerId == PlayerId)
			{
				OnIsPlayerConnected.ExecuteIfBound(FBool(true));
				return;
			}
		}
		OnIsPlayerConnected.ExecuteIfBound(FBool());
	});
	GetListInternal(PLAYERS_KEY, Delegate);
#endif
}

void FOnlineSessionSRO::GetConnectedPlayers()
{
#if WITH_SERVER_CODE
	const auto Delegate = FGetListInternalDelegate::CreateLambda([this](FListResponse Response, FString Error) {
		if (!Error.IsEmpty())
		{
			UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to get connected players: %s"), *Error);
			return;
		}
		
		OnGetConnectedPlayers.ExecuteIfBound(Response);
	});
	GetListInternal(PLAYERS_KEY, Delegate);
#endif
}

FHttpRequestRef FOnlineSessionSRO::BuildAgonesRequest(const FString& Path, const FHttpVerb Verb, const FString& Content)
{
	FHttpModule* Http = &FHttpModule::Get();
	FHttpRequestRef Request = Http->CreateRequest();

	Request->SetURL(FString::Format(
		TEXT("http://127.0.0.1:{0}/{1}"), 
{FStringFormatArg(AgonesPort), FStringFormatArg(Path)}	
	));
	Request->SetVerb(Verb.ToString());
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
	Request->SetContentAsString(Content);
	return Request;
}

FString FOnlineSessionSRO::IsValidResponse(const bool bSucceeded, const FHttpResponsePtr HttpResponse)
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

FString FOnlineSessionSRO::IsValidJsonResponse(TSharedPtr<FJsonObject>& JsonObject, const bool bSucceeded, const FHttpResponsePtr HttpResponse)
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

void FOnlineSessionSRO::GetListInternal(const FString& Key, FGetListInternalDelegate CompleteDelegate)
{
	FHttpRequestRef Request = BuildAgonesRequest(FString::Format(TEXT("v1beta1/lists/{0}"), {Key}), FHttpVerb::Get, "");
	Request->OnProcessRequestComplete().BindLambda(
		[CompleteDelegate](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			TSharedPtr<FJsonObject> JsonObject;
			FListResponse Response;
			
			const FString Error = IsValidJsonResponse(JsonObject, bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				CompleteDelegate.ExecuteIfBound(Response, Error);
				return;
			}
			
			if (!Response.FromJson(JsonObject))
			{
				CompleteDelegate.ExecuteIfBound(Response, TEXT("Failed to parse response"));
				return;
			}
			
			CompleteDelegate.ExecuteIfBound(Response, TEXT(""));
		});
	Request->ProcessRequest();
}

void FOnlineSessionSRO::Connect()
{
#if WITH_SERVER_CODE
	UE_LOG_ONLINE_SESSION(Log, TEXT("Connect request called "));
	GameServerRate = 5.f;
	GameServerTimer = -GameServerRate;
	GameServer();
#else
	UE_LOG_ONLINE_SESSION(Log, TEXT("Connect request ignored"));
#endif
}

void FOnlineSessionSRO::Ready()
{
#if WITH_SERVER_CODE
	FHttpRequestRef Request = BuildAgonesRequest("ready");
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to call ready: %s"), *Error);
				return;
			}
	
			OnReady.ExecuteIfBound();
		});
	Request->ProcessRequest();
#endif
}

void FOnlineSessionSRO::GameServer()
{
#if WITH_SERVER_CODE
	UE_LOG_ONLINE_SESSION(Log, TEXT("Starting GameServer request"));
	FHttpRequestRef Request = BuildAgonesRequest("gameserver", FHttpVerb::Get, "");
	Request->OnProcessRequestComplete().BindLambda(
		[this](FHttpRequestPtr HttpRequest, const FHttpResponsePtr HttpResponse, const bool bSucceeded) {
			UE_LOG_ONLINE_SESSION(Log, TEXT("GameServer response recieved"));
			FString Error = IsValidResponse(bSucceeded, HttpResponse);
			if (!Error.IsEmpty())
			{
				UE_LOG_ONLINE_SESSION(Warning, TEXT("Failed to get connected players: %s"), *Error);
				return;
			}
			
			UE_LOG_ONLINE_SESSION(Log, TEXT("GameServer response received"));
			CachedGameServer = FGameServer();
			CachedGameServer.Parse(HttpResponse->GetContentAsString());
			OnGameServer.ExecuteIfBound(CachedGameServer);
	
			if (!bIsAgonesConnected)
			{
				UE_LOG_ONLINE_SESSION(Log, TEXT("GameServer session created"));
				bIsAgonesConnected = true;
				FOnlineSessionSettings Settings;
				CreateSession(0, NAME_GameSession, Settings);
				UE_LOG_ONLINE_SESSION(Log, TEXT("GameServer session ready"));
				Ready();
			}
		});
	Request->ProcessRequest();
	UE_LOG_ONLINE_SESSION(Log, TEXT("GameServer request sent"));
#else
	UE_LOG_ONLINE_SESSION(Log, TEXT("GameServer request won't send"));
#endif
}

