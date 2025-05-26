// Copyright Epic Games, Inc. All Rights Reserved.

#include "SROLoginWindowStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FSROLoginWindowStyle::StyleInstance = nullptr;

void FSROLoginWindowStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FSROLoginWindowStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FSROLoginWindowStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("SROLoginWindowStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FSROLoginWindowStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("SROLoginWindowStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("SROLoginWindow")->GetBaseDir() / TEXT("Resources"));

	Style->Set("SROLoginWindow.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FSROLoginWindowStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FSROLoginWindowStyle::Get()
{
	return *StyleInstance;
}
