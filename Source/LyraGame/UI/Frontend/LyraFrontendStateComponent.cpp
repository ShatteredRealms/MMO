// Copyright Epic Games, Inc. All Rights Reserved.

#include "LyraFrontendStateComponent.h"

#include "CommonGameInstance.h"
#include "CommonSessionSubsystem.h"
#include "CommonUserSubsystem.h"
#include "ControlFlowManager.h"
#include "GameModes/LyraExperienceManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NativeGameplayTags.h"
#include "PrimaryGameLayout.h"
#include "ILoginFlowModule.h"
#include "ILoginFlowManager.h"
#include "LyraLogChannels.h"
#include "OnlineSubsystemSRO.h"
#include "OnlineSubsystemUtils.h"
#include "GameModes/LyraGameMode.h"
#include "GameModes/LyraGameState.h"
#include "Interfaces/OnlineExternalUIInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraFrontendStateComponent)

namespace FrontendTags
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PLATFORM_TRAIT_SINGLEONLINEUSER, "Platform.Trait.SingleOnlineUser");
	UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_UI_LAYER_MENU, "UI.Layer.Menu");
}

ULyraFrontendStateComponent::ULyraFrontendStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ULyraFrontendStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
	check(ExperienceComponent);

	// This delegate is on a component with the same lifetime as this one, so no need to unhook it in 
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_HighPriority(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	
	ILoginFlowModule& LoginFlowModule = ILoginFlowModule::Get();
	LoginFlowManager = LoginFlowModule.CreateLoginFlowManager();
	
	FName Identifier = SRO_SUBSYSTEM; 
#if UE_EDITOR // at present, multiple worlds are only possible in the editor
		Identifier = Online::GetUtils()->GetOnlineIdentifier(GetWorld(), SRO_SUBSYSTEM);
#endif

	if (LoginFlowManager->AddLoginFlow(
		Identifier,
		ILoginFlowManager::FOnDisplayPopup::CreateUObject(this, &ThisClass::OnDisplayLoginWidget),
		ILoginFlowManager::FOnDisplayPopup::CreateUObject(this, &ThisClass::OnDisplayCreationWidget)))
	{
		OnLoginPopupDismissed.BindUObject(this, &ThisClass::OnDismissLoginWidget);
		OnCreationPopupDismissed.BindUObject(this, &ThisClass::OnDismissLoginWidget);
	}
	else
	{
		UE_LOG(LogLyraExperience, Error, TEXT("Failed to add SRO login flow to LoginFlowManager"));
	}
}

void ULyraFrontendStateComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool ULyraFrontendStateComponent::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (bShouldShowLoadingScreen)
	{
		OutReason = TEXT("Frontend Flow Pending...");
		
		if (FrontEndFlow.IsValid())
		{
			const TOptional<FString> StepDebugName = FrontEndFlow->GetCurrentStepDebugName();
			if (StepDebugName.IsSet())
			{
				OutReason = StepDebugName.GetValue();
			}
		}
		
		return true;
	}

	return false;
}

void ULyraFrontendStateComponent::OnExperienceLoaded(const ULyraExperienceDefinition* Experience)
{
	FControlFlow& Flow = FControlFlowStatics::Create(this, TEXT("FrontendFlow"))
		.QueueStep(TEXT("Wait For User Initialization"), this, &ThisClass::FlowStep_WaitForUserInitialization)
		.QueueStep(TEXT("Try Show Main Screen"), this, &ThisClass::FlowStep_TryShowMainScreen);

	Flow.ExecuteFlow();

	FrontEndFlow = Flow.AsShared();
}

void ULyraFrontendStateComponent::FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow)
{
	// If this was a hard disconnect, explicitly destroy all user and session state
	// TODO: Refactor the engine disconnect flow so it is more explicit about why it happened
	bool bWasHardDisconnect = false;
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AGameModeBase>();
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);

	if (ensure(GameMode) && UGameplayStatics::HasOption(GameMode->OptionsString, TEXT("closed")))
	{
		bWasHardDisconnect = true;
	}

	// Only reset users on hard disconnect
	UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();
	if (ensure(UserSubsystem) && bWasHardDisconnect)
	{
		UserSubsystem->ResetUserState();
	}

	SubFlow->ContinueFlow();
}

void ULyraFrontendStateComponent::OnUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	FControlFlowNodePtr FlowToContinue = InProgressPressStartScreen;
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	UCommonUserSubsystem* UserSubsystem = GameInstance->GetSubsystem<UCommonUserSubsystem>();

	if (ensure(FlowToContinue.IsValid() && UserSubsystem))
	{
		UserSubsystem->OnUserInitializeComplete.RemoveDynamic(this, &ULyraFrontendStateComponent::OnUserInitialized);
		InProgressPressStartScreen.Reset();

		if (bSuccess)
		{
			// On success continue flow normally
			FlowToContinue->ContinueFlow();
		}
		else
		{
			// TODO: Just continue for now, could go to some sort of error screen
			FlowToContinue->ContinueFlow();
		}
	}
}

void ULyraFrontendStateComponent::FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow)
{
	if (UPrimaryGameLayout* RootLayout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(this))
	{
		constexpr bool bSuspendInputUntilComplete = true;
		RootLayout->PushWidgetToLayerStackAsync<UCommonActivatableWidget>(FrontendTags::TAG_UI_LAYER_MENU, bSuspendInputUntilComplete, MainScreenClass,
			[this, SubFlow](EAsyncWidgetLayerState State, UCommonActivatableWidget* Screen) {
			switch (State)
			{
			case EAsyncWidgetLayerState::AfterPush:
				bShouldShowLoadingScreen = false;
				SubFlow->ContinueFlow();
				return;
			case EAsyncWidgetLayerState::Canceled:
				bShouldShowLoadingScreen = false;
				SubFlow->ContinueFlow();
				return;
			}
		});
	}
}

ULyraFrontendStateComponent::FOnPopupDismissed ULyraFrontendStateComponent::OnDisplayLoginWidget(const TSharedRef<SWidget>& Widget)
{
	Overlay = SNew(SScrollBox)
	+SScrollBox::Slot().Padding(10,5)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot().Padding(5).HAlign(HAlign_Center)
		[
			SNew(SButton).Text(FText::FromString("Exit")).OnClicked_Lambda([this]
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(Overlay.ToSharedRef());
				OnLoginPopupDismissed.ExecuteIfBound();
				CancelLoginFlow();
				return FReply::Handled();
			})
		]
	]
	+SScrollBox::Slot().Padding(10,5).FillSize(1)
	[
		Widget
	];
	GEngine->GameViewport->AddViewportWidgetContent(Overlay.ToSharedRef());
	return OnLoginPopupDismissed;
}

ULyraFrontendStateComponent::FOnPopupDismissed  ULyraFrontendStateComponent::OnDisplayCreationWidget(const TSharedRef<SWidget>& Widget)
{
	Overlay = SNew(SScrollBox)
	+SScrollBox::Slot().Padding(10,5)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot().Padding(5).HAlign(HAlign_Center)
		[
			SNew(SButton).Text(FText::FromString("Exit")).OnClicked_Lambda([this]
			{
				GEngine->GameViewport->RemoveViewportWidgetContent(Overlay.ToSharedRef());
				OnCreationPopupDismissed.ExecuteIfBound();
				CancelLoginFlow();
				return FReply::Handled();
			})
		]
	]
	+SScrollBox::Slot().FillSize(1)
	[
		Widget
	];
	GEngine->GameViewport->AddViewportWidgetContent(Overlay.ToSharedRef());
	return OnCreationPopupDismissed;
}

void ULyraFrontendStateComponent::OnDismissLoginWidget()
{
	GEngine->GameViewport->RemoveViewportWidgetContent(Overlay.ToSharedRef());
}

FReply ULyraFrontendStateComponent::CancelLoginFlow()
{
	if (LoginFlowManager.IsValid())
	{
		LoginFlowManager->CancelLoginFlow();
		OnDismissLoginWidget(); // Remove this
	}

	return FReply::Handled();
}


