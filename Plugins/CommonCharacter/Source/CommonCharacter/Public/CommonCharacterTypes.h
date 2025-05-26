// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CommonCharacterTypes.generated.h"

USTRUCT(BlueprintType)
struct FSROCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CommonCharacter | Character")
	FString Id;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CommonCharacter | Character")
	FString DisplayName;
};