// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "SROLoginWindowStyle.h"

class FSROLoginWindowCommands : public TCommands<FSROLoginWindowCommands>
{
public:

	FSROLoginWindowCommands()
		: TCommands<FSROLoginWindowCommands>(TEXT("SROLoginWindow"), NSLOCTEXT("Contexts", "SROLoginWindow", "SROLoginWindow Plugin"), NAME_None, FSROLoginWindowStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};