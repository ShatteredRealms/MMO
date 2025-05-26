// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineAsyncTaskManager.h"

/**
 *	Google Play version of the async task manager
 */
class FOnlineAsyncTaskManagerSRO : public FOnlineAsyncTaskManager
{
public:

	FOnlineAsyncTaskManagerSRO() {}

	// FOnlineAsyncTaskManager
	virtual void OnlineTick() override {}
};


