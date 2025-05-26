// Copyright Epic Games, Inc. All Rights Reserved.

#include "SROLoginWindow.h"

#include "HttpModule.h"
#include "SROLoginWindowStyle.h"
#include "SROLoginWindowCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "ToolMenus.h"
#include "WebBrowserModule.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IHttpResponse.h"
#include "Runtime/WebBrowser/Public/SWebBrowser.h"

static const FName SROLoginWindowTabName("SROLoginWindow");

#define LOCTEXT_NAMESPACE "FSROLoginWindowModule"

void FSROLoginWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FSROLoginWindowStyle::Initialize();
	FSROLoginWindowStyle::ReloadTextures();

	FSROLoginWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FSROLoginWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FSROLoginWindowModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FSROLoginWindowModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(SROLoginWindowTabName, FOnSpawnTab::CreateRaw(this, &FSROLoginWindowModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FSROLoginWindowTabTitle", "SROLoginWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FSROLoginWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FSROLoginWindowStyle::Shutdown();

	FSROLoginWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(SROLoginWindowTabName);
}

TSharedRef<SDockTab> FSROLoginWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FSROLoginWindowModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("SROLoginWindow.cpp"))
		);

	FCreateBrowserWindowSettings Settings;
	Settings.bUseTransparency = false;
	Settings.BrowserFrameRate = 30;
	Settings.bShowErrorMessage = true;
	Settings.InitialURL = FString::Printf(
		TEXT("http://localhost:8080/realms/default/protocol/openid-connect/auth?client_id=sro-gameclient&response_type=code&scope=openid profile&redirect_uri=%s"),
		*FString(INTERNAL_REDIRECT_URL));

	SAssignNew(WebBrowser, SWebBrowser, IWebBrowserModule::Get().GetSingleton()->CreateBrowserWindow(Settings))
		.InitialURL(Settings.InitialURL)
		.SupportsTransparency(false)
		.ShowControls(false)
		.OnUrlChanged(FOnTextChanged::CreateRaw(this, &FSROLoginWindowModule::OnUrlChanged));

	SAssignNew(ThisTab, SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			WebBrowser.ToSharedRef()
		];
	return ThisTab.ToSharedRef();
}

void FSROLoginWindowModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(SROLoginWindowTabName);
}

void FSROLoginWindowModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FSROLoginWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FSROLoginWindowCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FSROLoginWindowModule::OnTokenRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (bWasSuccessful &&
		Response.IsValid() &&
		EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		if (AuthToken.Parse(Response->GetContentAsString()))
		{
			UE_LOG(LogTemp, Display, TEXT("FSROLoginWindowModule: RefreshToken acquired"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SROLoginWindow OnTokenRequestComplete: Invalid response"));
	}
}

void FSROLoginWindowModule::OnUrlChanged(const FText& Text)
{
	if (Text.ToString().StartsWith(INTERNAL_REDIRECT_URL))
	{
		TOptional<FString> Code = FGenericPlatformHttp::GetUrlParameter(Text.ToString(), TEXT("code"));
		
		const FString PostContent = FString::Printf(TEXT("code=%s&client_id=sro-gameclient&redirect_uri=%s&grant_type=authorization_code"),
			*Code.Get("a"),
			*FString(INTERNAL_REDIRECT_URL));

		// kick off http request to convert the exchange code to access token
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetURL(TEXT("http://localhost:8080/realms/default/protocol/openid-connect/token"));
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
		HttpRequest->SetVerb(TEXT("POST"));
		HttpRequest->SetContentAsString(PostContent);
		HttpRequest->OnProcessRequestComplete().BindRaw(this, &FSROLoginWindowModule::OnTokenRequestComplete);
		HttpRequest->ProcessRequest();

		ThisTab->RequestCloseTab();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSROLoginWindowModule, SROLoginWindow)