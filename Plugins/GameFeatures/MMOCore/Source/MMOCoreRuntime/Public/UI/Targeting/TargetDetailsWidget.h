// Copyright Wil Simpson All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSroCharacter/CharacterMessage.h"
#include "TargetDetailsWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UBorder;
class ULyraHealthComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class MMOCORERUNTIME_API UTargetDetailsWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Owner;
	
	UPROPERTY(BlueprintReadOnly)
	bool bTargeted;
	
	UPROPERTY(BlueprintReadOnly)
	bool bAttacked;

	UPROPERTY(BlueprintReadOnly)
	FString DisplayName;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor TargetedHighlightColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor AttackedHighlightColor;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULyraHealthComponent> HealthComponent;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UBorder> Border;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UTextBlock> NameTextBox;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetedChangedEvent(const bool bIsTargeted);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackedChangedEvent(const bool bIsTargeted);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDisplayNameChangedEvent(const FString& Name);

	UFUNCTION(BlueprintImplementableEvent)
	void OnHighlightColorChangedEvent(const FLinearColor& Color);

	UFUNCTION(BlueprintImplementableEvent)
	void OnSetupCompleted(AActor* NewOwner);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealthPercentage(ULyraHealthComponent* InHealthComponent, float OldValue, float NewValue, AActor* Instigator);
	
private:
	void UpdateBorder();
	
public:
	
	UFUNCTION()
	void OnCharaterDetailsChanged(const FGrpcSroCharacterCharacter& NewCharacterDetails, const FGrpcSroCharacterCharacter& OldCharacterDetails);
	
	virtual void Setup(AActor* InOwner);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetOwner() const { return Owner; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAttackable() const { return HealthComponent != nullptr; }
	
	UFUNCTION(BlueprintCallable)
	bool SetAttacked(bool bIsAttacked);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAttacked() const { return bAttacked; };

	UFUNCTION(BlueprintCallable)
	void SetDisplayName(FString NewDisplayName);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetDisplayName() const { return DisplayName; }
	
	UFUNCTION(BlueprintCallable, BlueprintCallable)
	bool SetTargeted(bool bNewTargeted);
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsTargeted() const { return bTargeted; }
};

