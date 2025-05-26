// Copyright Epic Games, Inc. All Rights Reserved.

#include "SROLoginWindowCommands.h"

#define LOCTEXT_NAMESPACE "FSROLoginWindowModule"

void FSROLoginWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SROLoginWindow", "Bring up SROLoginWindow window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
