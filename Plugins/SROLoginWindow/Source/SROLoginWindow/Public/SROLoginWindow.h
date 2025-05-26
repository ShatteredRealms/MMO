// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SWebBrowser.h"
#include "Interfaces/IHttpRequest.h"
#include "Modules/ModuleManager.h"
#include "AuthTypes.h"

#define INTERNAL_REDIRECT_URL TEXT("http://ue5login.local")

class FToolBarBuilder;
class FMenuBuilder;

class FSROLoginWindowModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	FORCEINLINE FAuthTokenSRO GetAuthToken() { return AuthToken; }
	
private:

	void RegisterMenus();

	void OnTokenRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnUrlChanged(const FText& Text);
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<SDockTab> ThisTab;
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SWebBrowser> WebBrowser;
	FAuthTokenSRO AuthToken;
};
