#pragma once

#include "SSroCharacter/CharacterClient.h"
#include "SSroCharacter/CharacterMessage.h"

#include "CommonCharacterSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAllCharactersChanged_Multicast, const TArray<FGrpcSroCharacterCharacter>&, Characters, FString, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnError_Multicast, FString, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCreated_Multicast, FGrpcSroCharacterCharacter, Character);

UCLASS(BlueprintType)
class COMMONCHARACTER_API UCommonCharacterSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UCommonCharacterSubsystem() { }
	
	virtual void Deinitialize() override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void ResetCharacterState();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CommonCharacter | Character")
	FGrpcSroCharacterCharacter GetCurrentCharacter() const;
	
	UFUNCTION(BlueprintCallable, Category="CommonCharacter | Character")
	void SetCurrentCharacter(const FGrpcSroCharacterCharacter& InCurrentCharacter);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CommonCharacter | Character")
	TArray<FGrpcSroCharacterCharacter> GetCharacters() const;

	UFUNCTION(BlueprintCallable, Category="CommonCharacter | Character")
	FString DeleteCharacter(const FString& CharacterId);

	UFUNCTION(BlueprintCallable, Category="CommonCharacter | Character")
	FString CreateCharacter(const FString& Name, const FString& Realm, const FString& Gender, const FString& Profession, const FString& DimensionId);

	UFUNCTION(BlueprintCallable)
	FString SearchAllCharacters();

	/** Broadcasts when AllCharacters changes */
	UPROPERTY(BlueprintAssignable, Category="CommonCharacter")
	FOnAllCharactersChanged_Multicast OnAllCharactersChanged;

	/** Broadcasts when an error occurs deleting a character */
	UPROPERTY(BlueprintAssignable, Category="CommonCharacter | Error")
	FOnError_Multicast OnCharacterDeletedError;

	/** Broadcasts when a character is created changes */
	UPROPERTY(BlueprintAssignable, Category="CommonCharacter")
	FOnCharacterCreated_Multicast OnCharacterCreated;
	
	/** Broadcasts when an error occurs deleting a character */
	UPROPERTY(BlueprintAssignable, Category="CommonCharacter | Error")
	FOnError_Multicast OnCharacterCreatedError;
	
protected:	
	////////////////////////////////////////////
	// Character Service Responses
	UFUNCTION()
	void OnGetCharacterResponseReceived(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacter& Response);
	
	UFUNCTION()
	void OnCreateCharacterResponseReceived(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacter& Response);
	
	UFUNCTION()
	void OnDeleteCharacterResponseReceived(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response);
	
	UFUNCTION()
	void OnGetCharactersResponseReceived(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacters& Response);
	
	UFUNCTION()
	void OnGetCharactersForUserResponseReceived(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacters& Response);
	// Character Service Responses
	////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CommonCharacter | Character")
	bool IsValidGender(const FString& Value) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CommonCharacter | Character")
	bool IsValidRealm(const FString& Value) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CommonCharacter | Character")
	bool IsValidProfession(const FString& Value) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CommonCharacter | Character")
	FString IsValidName(const FString& Value) const;
	
private:
	UCharacterServiceClient* CharacterServiceClient;

	FGrpcSroCharacterCharacter CurrentCharacter;

	UPROPERTY()
	TMap<FGrpcContextHandle, FGrpcSroCharacterCharacter> PendingDeleteCharacters;
	
	TArray<FGrpcSroCharacterCharacter> AllCharacters;

	const TSet<FString> Realms = {TEXT("human")};
	const TSet<FString> Genders = {TEXT("male"), TEXT("female")};
	const TSet<FString> Professions = {TEXT("necromancer")};
};
