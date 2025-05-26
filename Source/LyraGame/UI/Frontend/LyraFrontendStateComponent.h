// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/GameStateComponent.h"
#include "ControlFlowNode.h"
#include "LoadingProcessInterface.h"

#include "LyraFrontendStateComponent.generated.h"

class FControlFlow;
class FString;
class FText;
class UObject;
struct FFrame;

enum class ECommonUserOnlineContext : uint8;
enum class ECommonUserPrivilege : uint8;
class UCommonActivatableWidget;
class UCommonUserInfo;
class ULyraExperienceDefinition;
class ILoginFlowManager;
class SWidget;

UCLASS(Abstract)
class ULyraFrontendStateComponent : public UGameStateComponent, public ILoadingProcessInterface
{
	GENERATED_BODY()

public:

	ULyraFrontendStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	//~ILoadingProcessInterface interface
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	//~End of ILoadingProcessInterface

private:
	void OnExperienceLoaded(const ULyraExperienceDefinition* Experience);

	UFUNCTION()
	void OnUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext);

	void FlowStep_WaitForUserInitialization(FControlFlowNodeRef SubFlow);
	void FlowStep_TryShowMainScreen(FControlFlowNodeRef SubFlow);

	bool bShouldShowLoadingScreen = true;

	UPROPERTY(EditAnywhere, Category = UI)
	TSoftClassPtr<UCommonActivatableWidget> MainScreenClass;

	TSharedPtr<FControlFlow> FrontEndFlow;
	
	// If set, this is the in-progress press start screen task
	FControlFlowNodePtr InProgressPressStartScreen;

	FDelegateHandle OnJoinSessionCompleteEventHandle;

public:
	//////////////////////////////////
	// External Login UI Functionality
	
	DECLARE_DELEGATE(FOnPopupDismissed);
	FOnPopupDismissed OnDisplayLoginWidget(const TSharedRef<SWidget>& Widget);
	FOnPopupDismissed OnDisplayCreationWidget(const TSharedRef<SWidget>& Widget);
	void OnDismissLoginWidget();

	virtual FReply CancelLoginFlow();

	/** LoginFlowManager used to show login UI */
	TSharedPtr<ILoginFlowManager> LoginFlowManager;

	/** Overlay used to display the login flow */
	TSharedPtr<class SWidget> Overlay;

	/** Delegate called when login popup dismissed */
	FOnPopupDismissed OnLoginPopupDismissed;

	/** Delegate called when account creation popup is dismissed */
	FOnPopupDismissed OnCreationPopupDismissed;
};
