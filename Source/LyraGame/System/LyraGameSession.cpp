// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraGameSession.h"

#include "Online.h"
#include "GameFramework/PlayerState.h"
#include "Net/OnlineEngineInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraGameSession)


ALyraGameSession::ALyraGameSession(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool ALyraGameSession::ProcessAutoLogin()
{
	// This is actually handled in LyraGameMode::TryDedicatedServerLogin
	return true;
}

void ALyraGameSession::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

void ALyraGameSession::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
}

void ALyraGameSession::UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId)
{
	if (GetNetMode() != NM_Standalone &&
		UniqueId.IsValid())
	{
		Online::GetSessionInterface()->UnregisterPlayer(NAME_GameSession, *UniqueId);
	}
}

void ALyraGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
	if (IsRunningDedicatedServer())
	{
		Online::GetSessionInterface()->RegisterPlayer(NAME_GameSession, *UniqueId, bWasFromInvite);
	}
}
