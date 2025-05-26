#pragma once
#include "OnlineJsonSerializer.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/OnlineSessionInterface.h"

class FOnlineSubsystemSRO;

static const FString PLAYERS_KEY = TEXT("players");
static const FString USERS_KEY = TEXT("users");
static FName SRO_SESSION_ID = TEXT("SRO_SESSION_ID");
TEMP_UNIQUENETIDSTRING_SUBCLASS(FUniqueNetIdSROSession, SRO_SESSION_ID);

class FInitializedJson :
	public FOnlineJsonSerializable
{
public:
	FInitializedJson()
		: bInitialized(false)
	{
	}

	/** @return true if this data is valid, false otherwise */
	bool IsValid() const { return bInitialized; }

	/**
	 * Parse a Json response from SRO into a this data structure
	 *
	 * @return true if parsing was successful, false otherwise
	 */
	bool Parse(const FString& InJsonStr)
	{
		if (!InJsonStr.IsEmpty() && FromJson(InJsonStr))
		{
			bInitialized = true;
		}

		return bInitialized;
	}

	/** Has this data been setup */
	bool bInitialized;
};

class FObjectMeta :
	public FInitializedJson
{
public:
	FObjectMeta()
	{
	}

	FString Name;
	FString Namespace;
	FString Uid;
	FString ResourceVersion;
	int64 Generation = 0;
	int64 CreationTimestamp = 0;
	int64 DeletionTimestamp = 0;
	TMap<FString, FString> Annotations;
	TMap<FString, FString> Labels;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("name", Name);
		ONLINE_JSON_SERIALIZE("namespace", Namespace);
		ONLINE_JSON_SERIALIZE("uid", Uid);
		ONLINE_JSON_SERIALIZE("resource_version", ResourceVersion);
		ONLINE_JSON_SERIALIZE("generation", Generation);
		ONLINE_JSON_SERIALIZE("CreationTimestamp", CreationTimestamp);
		ONLINE_JSON_SERIALIZE("DeletionTimestamp", DeletionTimestamp);
		ONLINE_JSON_SERIALIZE_SIMPLE_COPY(Annotations);
		ONLINE_JSON_SERIALIZE_SIMPLE_COPY(Labels);
	END_ONLINE_JSON_SERIALIZER
};

class FHealth :
	public FInitializedJson
{
public:
	FHealth()
	{
	}

	bool bDisabled = false;
	int32 PeriodSeconds = 0;
	int32 FailureThreshold = 0;
	int32 InitialDelaySeconds = 0;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("disabled", bDisabled);
		ONLINE_JSON_SERIALIZE("period_seconds", PeriodSeconds);
		ONLINE_JSON_SERIALIZE("failure_threshold", FailureThreshold);
		ONLINE_JSON_SERIALIZE("initial_delay_seconds", InitialDelaySeconds);
	END_ONLINE_JSON_SERIALIZER
};

class FSpec :
	public FInitializedJson
{
public:
	FSpec()
	{
	}

	FHealth Health;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE_OBJECT_SERIALIZABLE("health", Health);
	END_ONLINE_JSON_SERIALIZER
};

struct FListResponse :
	public FOnlineJsonSerializable
{
	FListResponse()
	{
	}

	TArray<FString> List = TArray<FString>();
	int64 Capacity = 0;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE_ARRAY("list", List);
		ONLINE_JSON_SERIALIZE("capacity", Capacity);
	END_ONLINE_JSON_SERIALIZER
};

class FConnectedPlayers :
	public FInitializedJson
{
public:
	FConnectedPlayers()
	{
	}

	TArray<FString> ConnectedPlayers;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE_ARRAY("list", ConnectedPlayers);
	END_ONLINE_JSON_SERIALIZER
};

class FPlayerCount :
	public FOnlineJsonSerializable
{
public:
	FPlayerCount(int64 InCount)
		: Count(InCount)
	{
	}

	FPlayerCount(FString InJson)
	{
		FromJson(InJson);
	}

	int64 Count;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("count", Count);
	END_ONLINE_JSON_SERIALIZER
};

class FBool :
	public FInitializedJson
{
public:
	FBool()
		: bBool(false)
	{
	}
	
	FBool(bool val)
		: bBool(val)
	{
		bInitialized = true;
	}

	bool bBool;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("bool", bBool);
	END_ONLINE_JSON_SERIALIZER
};

class FPort :
	public FInitializedJson
{
public:
	FPort()
		: Name(""), Port(0)
	{
	}

	FString Name;
	int32 Port;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("name", Name);
		ONLINE_JSON_SERIALIZE("port", Port);
	END_ONLINE_JSON_SERIALIZER
};

class FStatus :
	public FInitializedJson
{
public:
	FStatus() : State(TEXT("")), Address(TEXT("")), Ports(TArray<FPort>())
	{
	}

	FString State;
	FString Address;
	TArray<FPort> Ports;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("state", State);
		ONLINE_JSON_SERIALIZE("address", Address);
		ONLINE_JSON_SERIALIZE_ARRAY_SERIALIZABLE("ports", Ports, FPort);
	END_ONLINE_JSON_SERIALIZER
};


class FGameServer :
	public FInitializedJson
{
public:
	FGameServer() : Status(TEXT("")), ObjectMeta(FObjectMeta()), Spec(FSpec())
	{
	}

	FString Status;
	FObjectMeta ObjectMeta;
	FSpec Spec;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("status", Status);
		ONLINE_JSON_SERIALIZE_OBJECT_SERIALIZABLE("object_meta", ObjectMeta);
		ONLINE_JSON_SERIALIZE_OBJECT_SERIALIZABLE("spec", Spec);
	END_ONLINE_JSON_SERIALIZER
};

class FKVPair :
	public FOnlineJsonSerializable
{
public:
	FKVPair(FString InKey, FString InValue)
		: Key(InKey)
		  , Value(InValue)
	{
	}

	FString Key;
	FString Value;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("key", Key);
		ONLINE_JSON_SERIALIZE("value", Value);
	END_ONLINE_JSON_SERIALIZER
};

class FAgonesError
{
public:
	FAgonesError()
	{
	}

	FAgonesError(FString Error) : Error(Error)
	{
	}

	FString Error;
};

class FDuration :
	public FOnlineJsonSerializable
{
public:
	FDuration(int64 InSeconds) : Seconds(InSeconds)
	{
	}

	int64 Seconds;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("seconds", Seconds);
	END_ONLINE_JSON_SERIALIZER
};

class FAgonesPlayer :
	public FOnlineJsonSerializable
{
public:
	FAgonesPlayer(FString InPlayerId) : PlayerId(InPlayerId)
	{
	}

	FString PlayerId;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("playerID", PlayerId);
	END_ONLINE_JSON_SERIALIZER
};

class FHttpVerb
{
public:
	enum EVerb
	{
		Get,
		Post,
		Put,
		Patch
	};

	// ReSharper disable once CppNonExplicitConvertingConstructor
	FHttpVerb(const EVerb Verb) : Verb(Verb)
	{
	}

	FString ToString() const
	{
		switch (Verb)
		{
		case Post:
			return TEXT("POST");
		case Put:
			return TEXT("PUT");
		case Patch:
			return TEXT("PATCH");
		case Get:
		default:
			return TEXT("GET");
		}
	}

private:
	const EVerb Verb;
};

#define INI_SECTION TEXT("OnlineSubsystemSRO.OnlineSessionSRO")
#define AGONES_PORT TEXT("AgonesPort")
#define HEALTH_RATE TEXT("HealthRate")

DECLARE_DELEGATE_OneParam(FAgonesErrorDelegate, const FAgonesError&);
DECLARE_DELEGATE_OneParam(FGameServerDelegate, const FGameServer&);
DECLARE_DELEGATE_OneParam(FGetConnectedPlayersDelegate, const FListResponse&);
DECLARE_DELEGATE_OneParam(FPlayerCountDelegate, const FPlayerCount&);
DECLARE_DELEGATE_OneParam(FBoolDelegate, const FBool&);

/**
 * Interface for interacting with EOS sessions
 */
class FOnlineSessionSRO
	: public IOnlineSession
	  , public TSharedFromThis<FOnlineSessionSRO, ESPMode::ThreadSafe>
{
public:
	virtual FNamedOnlineSession* GetNamedSession(FName SessionName) override
	{
		return NamedOnlineSession;
	}

	virtual void RemoveNamedSession(FName SessionName) override
	{
		Shutdown();
		NamedOnlineSession = nullptr;
	}

	virtual bool HasPresenceSession() override
	{
		return false;
	};

	virtual EOnlineSessionState::Type GetSessionState(FName SessionName) const override
	{
		if (NamedOnlineSession != nullptr)
		{
			return NamedOnlineSession->SessionState;
		}

		return EOnlineSessionState::Type::NoSession;
	};

	FOnlineSessionSRO() = delete;
	virtual ~FOnlineSessionSRO();

	FOnlineSessionSRO(FOnlineSubsystemSRO* InSubsystem);

	virtual FUniqueNetIdPtr CreateSessionIdFromString(const FString& SessionIdStr) override;

	// IOnlineSession Interface
	virtual bool CreateSession(int32 HostingPlayerNum, FName SessionName,
	                           const FOnlineSessionSettings& NewSessionSettings) override;
	virtual bool CreateSession(const FUniqueNetId& HostingPlayerId, FName SessionName,
	                           const FOnlineSessionSettings& NewSessionSettings) override;
	virtual bool StartSession(FName SessionName) override;
	virtual bool UpdateSession(FName SessionName, FOnlineSessionSettings& UpdatedSessionSettings,
	                           bool bShouldRefreshOnlineData = true) override;
	virtual bool EndSession(FName SessionName) override;
	virtual bool DestroySession(FName SessionName,
	                            const FOnDestroySessionCompleteDelegate& CompletionDelegate =
		                            FOnDestroySessionCompleteDelegate()) override;
	virtual bool IsPlayerInSession(FName SessionName, const FUniqueNetId& UniqueId) override;
	virtual bool StartMatchmaking(const TArray<FUniqueNetIdRef>& LocalPlayers, FName SessionName,
	                              const FOnlineSessionSettings& NewSessionSettings,
	                              TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool CancelMatchmaking(int32 SearchingPlayerNum, FName SessionName) override;
	virtual bool CancelMatchmaking(const FUniqueNetId& SearchingPlayerId, FName SessionName) override;
	virtual bool
	FindSessions(int32 SearchingPlayerNum, const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool FindSessions(const FUniqueNetId& SearchingPlayerId,
	                          const TSharedRef<FOnlineSessionSearch>& SearchSettings) override;
	virtual bool FindSessionById(const FUniqueNetId& SearchingUserId, const FUniqueNetId& SessionId,
	                             const FUniqueNetId& FriendId,
	                             const FOnSingleSessionResultCompleteDelegate& CompletionDelegate) override;
	virtual bool CancelFindSessions() override;
	virtual bool PingSearchResults(const FOnlineSessionSearchResult& SearchResult) override;
	virtual bool JoinSession(int32 PlayerNum, FName SessionName,
	                         const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool JoinSession(const FUniqueNetId& PlayerId, FName SessionName,
	                         const FOnlineSessionSearchResult& DesiredSession) override;
	virtual bool FindFriendSession(int32 LocalUserNum, const FUniqueNetId& Friend) override;
	virtual bool FindFriendSession(const FUniqueNetId& LocalUserId, const FUniqueNetId& Friend) override;
	virtual bool FindFriendSession(const FUniqueNetId& LocalUserId, const TArray<FUniqueNetIdRef>& FriendList) override;
	virtual bool SendSessionInviteToFriend(int32 LocalUserNum, FName SessionName, const FUniqueNetId& Friend) override;
	virtual bool SendSessionInviteToFriend(const FUniqueNetId& LocalUserId, FName SessionName,
	                                       const FUniqueNetId& Friend) override;
	virtual bool SendSessionInviteToFriends(int32 LocalUserNum, FName SessionName,
	                                        const TArray<FUniqueNetIdRef>& Friends) override;
	virtual bool SendSessionInviteToFriends(const FUniqueNetId& LocalUserId, FName SessionName,
	                                        const TArray<FUniqueNetIdRef>& Friends) override;
	virtual bool GetResolvedConnectString(FName SessionName, FString& ConnectInfo, FName PortType) override;
	virtual bool GetResolvedConnectString(const FOnlineSessionSearchResult& SearchResult, FName PortType,
	                                      FString& ConnectInfo) override;
	virtual FOnlineSessionSettings* GetSessionSettings(FName SessionName) override;
	virtual FString GetVoiceChatRoomName(int32 LocalUserNum, const FName& SessionName) override;
	virtual bool RegisterPlayer(FName SessionName, const FUniqueNetId& PlayerId, bool bWasInvited) override;
	virtual bool RegisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players,
	                             bool bWasInvited = false) override;
	virtual bool UnregisterPlayer(FName SessionName, const FUniqueNetId& PlayerId) override;
	virtual bool UnregisterPlayers(FName SessionName, const TArray<FUniqueNetIdRef>& Players) override;
	virtual void RegisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName,
	                                 const FOnRegisterLocalPlayerCompleteDelegate& Delegate) override;
	virtual void UnregisterLocalPlayer(const FUniqueNetId& PlayerId, FName SessionName,
	                                   const FOnUnregisterLocalPlayerCompleteDelegate& Delegate) override;
	virtual void
	RemovePlayerFromSession(int32 LocalUserNum, FName SessionName, const FUniqueNetId& TargetPlayerId) override;
	virtual int32 GetNumSessions() override;
	virtual void DumpSessionState() override;
	// ~IOnlineSession Interface

	/** Critical sections for thread safe operation of session lists */
	mutable FCriticalSection SessionLock;

	/** Current session settings */
	TArray<FNamedOnlineSession> Sessions;

	/** Current search object */
	TSharedPtr<FOnlineSessionSearch> CurrentSessionSearch;

	/** Current search start time. */
	double SessionSearchStartInSeconds;

	/**
	 * Session tick for various background tasks
	 */
	void Tick(float DeltaTime);

	// IOnlineSession
	virtual class FNamedOnlineSession*
	AddNamedSession(FName SessionName, const FOnlineSessionSettings& SessionSettings) override
	{
		return NamedOnlineSession;
	}

	virtual class FNamedOnlineSession* AddNamedSession(FName SessionName, const FOnlineSession& Session) override
	{
		return NamedOnlineSession;
	}

	void HealthCheck();
	void Connect();
	void Allocate();
	void GameServer();
	void Health();
	void Ready();
	void Reserve(int64 Seconds);
	void SetAnnotation(const FString& Key, const FString& Value);
	void SetLabel(const FString& Key, const FString& Value);
	void Shutdown();
	void GetConnectedPlayers();
	void GetPlayerCapacity();
	void GetPlayerCount();
	void IsPlayerConnected(FString PlayerId);
	void PlayerConnect(FString PlayerId);
	void PlayerDisconnect(FString PlayerId);
	void SetPlayerCapacity(int64 Count);
	FHttpRequestRef BuildAgonesRequest(const FString& Path, const FHttpVerb Verb = FHttpVerb::Post,
	                                   const FString& Content = "{}");


	FGetConnectedPlayersDelegate OnGetConnectedPlayers;
	FBoolDelegate OnIsPlayerConnected;
	FPlayerCountDelegate OnGetPlayerCount;
	FPlayerCountDelegate OnGetPlayerCapacity;
	FSimpleDelegate OnSetPlayerCapacity;
	FBoolDelegate OnPlayerDisconnect;
	FBoolDelegate OnPlayerConnect;
	FSimpleDelegate OnReserve;
	FSimpleDelegate OnAllocate;
	FSimpleDelegate OnSetAnnotation;
	FSimpleDelegate OnShutdown;
	FSimpleDelegate OnSetLabel;
	FGameServerDelegate OnGameServer;
	FSimpleDelegate OnReady;

protected:
	/** HTTP port to use to connect with Agones sidecar */
	FString AgonesPort = "9358";

	/** How often to check the health of the game server. A value of 0 will disable auto health calls. */
	float HealthRate = 10.0f;

	TSet<FUniqueNetIdRef> ConnectedPlayers;

private:
	/** Reference to the main SRO subsystem */
	FOnlineSubsystemSRO* SROSubsystem;

	/** Time since the last Health function call */
	float HealthTimer = -HealthRate;

	/** The rate at which the GameServer function is called. This will be zero until the Connect is successful. */
	float GameServerRate = 0.f;

	/** Time since the last GameServer function call */
	float GameServerTimer = 0;

	/** Whether Agones has been connected to at least once. */
	bool bIsAgonesConnected = false;

	FGameServer CachedGameServer;

	FNamedOnlineSession* NamedOnlineSession = nullptr;

	static FString IsValidResponse(const bool bSucceeded, const FHttpResponsePtr HttpResponse);
	static FString IsValidJsonResponse(TSharedPtr<FJsonObject>& JsonObject, const bool bSucceeded, const FHttpResponsePtr HttpResponse);
	
	DECLARE_DELEGATE_TwoParams(FGetListInternalDelegate, const FListResponse&, const FString&);
	void GetListInternal(const FString& Key, FGetListInternalDelegate CompleteDelegate);
};
