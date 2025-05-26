// Copyright Shattered Realms Online 2024

#pragma once

#include "CoreMinimal.h"
#include "DataTypes.h"
#include "HTTPVerbs.h"
#include "IWebSocket.h"
#include "Interfaces/IHttpRequest.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "AgonesSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FAgonesErrorDelegate, const FAgonesError&, Error);
DECLARE_DYNAMIC_DELEGATE(FAllocateDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGameServerDelegate, const FGameServer&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetConnectedPlayersDelegate, const FConnectedPlayers, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetPlayerCapacityDelegate, const FCount, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetPlayerCountDelegate, const FCount, Response);
DECLARE_DYNAMIC_DELEGATE(FHealthDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FIsPlayerConnectedDelegate, const FBool, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPlayerConnectDelegate, const FBool, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FPlayerDisconnectDelegate, const FBool, Response);
DECLARE_DYNAMIC_DELEGATE(FReadyDelegate);
DECLARE_DYNAMIC_DELEGATE(FReserveDelegate);
DECLARE_DYNAMIC_DELEGATE(FSetAnnotationDelegate);
DECLARE_DYNAMIC_DELEGATE(FSetLabelDelegate);
DECLARE_DYNAMIC_DELEGATE(FSetPlayerCapacityDelegate);
DECLARE_DYNAMIC_DELEGATE(FGetCounterDelegate);
DECLARE_DYNAMIC_DELEGATE(FIncrementCounterDelegate);
DECLARE_DYNAMIC_DELEGATE(FDecrementCounterDelegate);
DECLARE_DYNAMIC_DELEGATE(FSetCounterCountDelegate);
DECLARE_DYNAMIC_DELEGATE(FSetCounterCapacityDelegate);
DECLARE_DYNAMIC_DELEGATE(FAppendListValueDelegate);
DECLARE_DYNAMIC_DELEGATE(FDeleteListValueDelegate);
DECLARE_DYNAMIC_DELEGATE(FSetListCapacityDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetListDelegate, const FList, Response);
DECLARE_DYNAMIC_DELEGATE(FListContainsDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetListLengthDelegate, const FCount, Response);
DECLARE_DYNAMIC_DELEGATE(FShutdownDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FConnectedDelegate, const FGameServer, Response);

static const FString PLAYERS_KEY = TEXT("players");
static const FString USERS_KEY = TEXT("users");

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Agones Subsystem"))
class AGONESSUBSYSTEM_API UAgonesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/**
	 * \brief HttpPort is the default Agones HTTP port to use.
	 */
	UPROPERTY(EditAnywhere, Category = Agones, Config)
	FString HttpPort = "9358";

	/**
	 * \brief HealthRateSeconds is the frequency to send Health calls. Value of 0 will disable auto health calls.
	 */
	UPROPERTY(EditAnywhere, Category = Agones, Config)
	float HealthRateSeconds = 10.f;

	/**
	 * \brief bDisableAutoConnect will stop the component auto connecting (calling GamesServer and Ready).
	 */
	UPROPERTY(EditAnywhere, Category = Agones, Config)
	bool bDisableAutoConnect;

	/**
	 * \brief ConnectedDelegate will be called once the Connect func gets a successful response from GameServer.
	 */
	UPROPERTY(BlueprintAssignable, Category = Agones)
	FConnectedDelegate ConnectedDelegate;

	/**
	 * \brief HealthPing loops calling the Health endpoint.
	 * \param RateSeconds rate at which the Health endpoint should be called.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Utility")
	void StartHealthPings(float RateSeconds);

	/**
	 * \brief Connect will call /gameserver till a successful response then call /ready
	 * a delegate is called with the gameserver response after /ready call is made.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Utility")
	void Connect();

	/**
	 * \brief Allocate self marks this gameserver as Allocated.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Lifecycle")
	void Allocate(FAllocateDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief GameServer retrieve the GameServer details.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Configuration")
	void GameServer(FGameServerDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief WatchGameServer subscribes a delegate to be called whenever game server details change.
	 * \param WatchDelegate - Called every time the game server data changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Configuration")
	void WatchGameServer(FGameServerDelegate WatchDelegate);

	/**
	 * \brief Health sends a ping to the health check to indicate that this server is healthy.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Lifecycle")
	void Health(FHealthDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief Ready marks the Game Server as ready to receive connections.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Lifecycle")
	void Ready(FReadyDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief Reserve marks the Game Server as Reserved for a given duration.
	 * \param Seconds - Seconds that the Game Server will be reserved.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Lifecycle")
	void Reserve(int64 Seconds, FReserveDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief SetAnnotation sets a metadata annotation on the `GameServer` with the prefix 'agones.dev/sdk-'
	 * calling SetAnnotation("foo", "bar", {}, {}) will result in the annotation "agones.dev/sdk-foo: bar".
	 * \param Key
	 * \param Value
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Metadata")
	void SetAnnotation(const FString& Key, const FString& Value, FSetAnnotationDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief SetLabel sets a metadata label on the `GameServer` with the prefix 'agones.dev/sdk-'
	 * calling SetLabel("foo", "bar", {}, {}) will result in the label "agones.dev/sdk-foo: bar".
	 * \param Key
	 * \param Value
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Metadata")
	void SetLabel(const FString& Key, const FString& Value, FSetLabelDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief Shutdown marks the Game Server as ready to shutdown
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Lifecycle")
	void Shutdown(FShutdownDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] GetConnectedPlayers returns the list of the currently connected player ids.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void GetConnectedPlayers(FGetConnectedPlayersDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] GetPlayerCapacity gets the last player capacity that was set through the SDK.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void GetPlayerCapacity(FGetPlayerCapacityDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] GetPlayerCount returns the current player count
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void GetPlayerCount(FGetPlayerCountDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] IsPlayerConnected returns if the playerID is currently connected to the GameServer.
	 * \param PlayerId - PlayerID of player to check.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void IsPlayerConnected(FString PlayerId, FIsPlayerConnectedDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] PlayerConnect increases the SDK’s stored player count by one, and appends this playerID to status.players.id.
	 * \param PlayerId - PlayerID of connecting player.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void PlayerConnect(FString PlayerId, FPlayerConnectDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] PlayerDisconnect Decreases the SDK’s stored player count by one, and removes the playerID from
	 * status.players.id.
	 *
	 * \param PlayerId - PlayerID of disconnecting player.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void PlayerDisconnect(FString PlayerId, FPlayerDisconnectDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	/**
	 * \brief [Alpha] SetPlayerCapacity changes the player capacity to a new value.
	 * \param Count - Capacity of game server.
	 * \param SuccessDelegate - Called on Successful call.
	 * \param ErrorDelegate - Called on Unsuccessful call.
	 */
	UFUNCTION(BlueprintCallable, Category = "Agones | Alpha | Player Tracking")
	void SetPlayerCapacity(int64 Count, FSetPlayerCapacityDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

	// /**
	//  * \brief [Beta] GetCounter return counter (count and capacity) associated with a Key.
	//  * \param Key - Key to counter value
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Counters")
	// void GetCounter(FString Key, FGetCounterDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] IncrementCounter incremenets counter associated with a Key by 1.
	//  * \param Key - Key to counter value
	//  * \param Amount - Amount that would be added to count.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Counters")
	// void IncrementCounter(FString Key, int64 Amount, FIncrementCounterDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] DecrementCounter decremenets counter associated with a Key by 1.
	//  * \param Key - Key to counter value
	//  * \param Amount - Amount that would be decremented from count.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Counters")
	// void DecrementCounter(FString Key, int64 Amount, FDecrementCounterDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] SetCounterCount set counter count associated with a Key.
	//  * \param Key - Key to counter value
	//  * \param Count - Active sessions count.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Counters")
	// void SetCounterCount(FString Key, int64 Count, FSetCounterCountDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] SetCounterCount set counter capacity associated with a Key.
	//  * \param Key - Key to counter value
	//  * \param Capacity - Capacity of game server.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Counters")
	// void SetCounterCapacity(FString Key, int64 Capacity, FSetCounterCapacityDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] AppendListValue append a value to a list associated with a Key.
	//  * \param Key - Key to list value
	//  * \param Value - Value to append to list.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Lists")
	// void AppendListValue(FString Key, FString Value, FAppendListValueDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] DeleteListValue remove a value to a list associated with a Key.
	//  * \param Key - Key to list value
	//  * \param Value - Value to remove from the list.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Lists")
	// void DeleteListValue(FString Key, FString Value, FDeleteListValueDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] SetListCapacity sets the maximum capacity for a list associated with a Key. 
	//  * \param Key - Key to list value
	//  * \param Amount - Amount to set capacity to.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Lists")
	// void SetListCapacity(FString Key, int64 Amount, FSetListCapacityDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] GetList gets the maximum capacity and values for a list associated with a Key. 
	//  * \param Key - Key to list value
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Lists")
	// void GetList(FString Key, FGetListDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] ListContains checks if the specific string is associated with a Key. 
	//  * \param Key - Key to list value
	//  * \param Value - Value to check if it exists in the list.
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Lists")
	// void ListContains(FString Key, FString Value, FListContainsDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);
	//
	// /**
	//  * \brief [Beta] GetListLength gets the length of the list with the associated with a Key. 
	//  * \param Key - Key to list value
	//  * \param SuccessDelegate - Called on Successful call.
	//  * \param ErrorDelegate - Called on Unsuccessful call.
	//  */
	// UFUNCTION(BlueprintCallable, Category = "Agones | Beta | Lists")
	// void GetListLength(FString Key, FGetListLengthDelegate SuccessDelegate, FAgonesErrorDelegate ErrorDelegate);

private:
	FTimerHandle ConnectTimerHandle;
	FTimerHandle HealthTimerHandle;
	FTimerHandle WebSocketTimerHandle;

	TSharedPtr<IWebSocket> WebSocket;
	TArray<UTF8CHAR> WatchMessageBuffer;
	TArray<FGameServerDelegate> WatchGameServerDelegates;
	
	FHttpRequestRef BuildAgonesRequest(const FString& Path, const FHttpVerb Verb = FHttpVerb::Post,
	                                   const FString& Content = "{}");

	static FString IsValidResponse(const bool bSucceeded, const FHttpResponsePtr HttpResponse);
	static FString IsValidJsonResponse(TSharedPtr<FJsonObject>& JsonObject, const bool bSucceeded, const FHttpResponsePtr HttpResponse);

	UFUNCTION(BlueprintInternalUseOnly)
	void HealthPingFailed(FAgonesError& Error);
	
	UFUNCTION(BlueprintInternalUseOnly)
	void ConnectSuccess(FGameServer GameServerResponse);
	
	DECLARE_DELEGATE_TwoParams(FGetListInternalDelegate, const FList&, const FString&);
	void GetListInternal(const FString& Key, FGetListInternalDelegate CompleteDelegate);
};
