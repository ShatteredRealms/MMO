// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/GameSession.h"

#include "LyraGameSession.generated.h"

class UObject;


UCLASS(Config = Game)
class ALyraGameSession : public AGameSession
{
	GENERATED_BODY()

public:

	ALyraGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	/** Override to disable the default behavior */
	virtual bool ProcessAutoLogin() override;

	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;

	virtual void UnregisterPlayer(FName InSessionName, const FUniqueNetIdRepl& UniqueId) override;
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;
};
