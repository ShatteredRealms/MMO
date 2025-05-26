#include "CommonCharacterSubsystem.h"

#include "OnlineSubsystemUtils.h"
#include "SSroCharacter/CharacterClient.h"
#include "SSroCharacter/CharacterMessage.h"
#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcUtilities.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "SSroCharacter/CharacterService.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonCharacterSubsystem)

DECLARE_LOG_CATEGORY_EXTERN(LogCommonCharacter, Log, All);
DEFINE_LOG_CATEGORY(LogCommonCharacter);

void UCommonCharacterSubsystem::Deinitialize()
{
	CharacterServiceClient->Shutdown();
}

void UCommonCharacterSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UTurboLinkGrpcManager* TLM = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(GetWorld());
	if (!TLM)
	{
		UE_LOG(LogCommonCharacter, Error, TEXT("Unable to create TLM"));
		return;
	}

	auto CharacterService = Cast<UCharacterService>(TLM->MakeService("CharacterService"));
	if (!CharacterService)
	{
		UE_LOG(LogCommonCharacter, Error, TEXT("Unable to create character service"));
		return;
	}

	CharacterServiceClient = CharacterService->MakeClient();
	CharacterServiceClient->OnGetCharacterResponse.AddUniqueDynamic(this, &ThisClass::OnGetCharacterResponseReceived);
	CharacterServiceClient->OnCreateCharacterResponse.AddUniqueDynamic(this, &ThisClass::OnCreateCharacterResponseReceived);
	CharacterServiceClient->OnDeleteCharacterResponse.AddUniqueDynamic(this, &ThisClass::OnDeleteCharacterResponseReceived);
	CharacterServiceClient->OnGetCharactersResponse.AddUniqueDynamic(this, &ThisClass::OnGetCharactersResponseReceived);
	CharacterServiceClient->OnGetCharactersForUserResponse.AddUniqueDynamic(this, &ThisClass::OnGetCharactersForUserResponseReceived);
	CharacterService->Connect();
}

bool UCommonCharacterSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UCommonCharacterSubsystem::ResetCharacterState()
{
	CurrentCharacter = FGrpcSroCharacterCharacter();
}

FGrpcSroCharacterCharacter UCommonCharacterSubsystem::GetCurrentCharacter() const
{
	return CurrentCharacter;
}

void UCommonCharacterSubsystem::SetCurrentCharacter(const FGrpcSroCharacterCharacter& InCurrentCharacter)
{
	CurrentCharacter = InCurrentCharacter;
}

TArray<FGrpcSroCharacterCharacter> UCommonCharacterSubsystem::GetCharacters() const
{
	return AllCharacters;
}

FString UCommonCharacterSubsystem::DeleteCharacter(const FString& CharacterId)
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetWorld());
	if (Identity->GetLoginStatus(0) != ELoginStatus::LoggedIn)
	{
		return TEXT("Not logged in");
	}

	FGrpcSroCharacterCharacter* Character = AllCharacters.FindByPredicate(
		[&CharacterId](const FGrpcSroCharacterCharacter& Character)
		{
			return Character.Id == CharacterId;
		});
	if (Character == nullptr)
	{
		return TEXT("Character not found");
	}
	
	const auto Handle = CharacterServiceClient->InitDeleteCharacter();
	FGrpcSroTargetId TargetId;
	TargetId.Id = CharacterId;
	auto MetaData = UTurboLinkGrpcUtilities::CreateAuthMetaData(Identity->GetAuthToken(0));
	
	PendingDeleteCharacters.Add(Handle, *Character);
	CharacterServiceClient->DeleteCharacter(Handle, TargetId, MetaData);

	return TEXT("");
}

FString UCommonCharacterSubsystem::CreateCharacter(const FString& Name, const FString& Realm, const FString& Gender, const FString& Profession, const FString& DimensionId)
{
	FString Error = IsValidName(Name);
	if (!Error.IsEmpty())
	{
		return Error;
	}

	if (!IsValidRealm(Realm))
	{
		return TEXT("Invalid realm");
	}

	if (!IsValidGender(Gender))
	{
		return TEXT("Invalid gender");
	}

	if (!IsValidProfession(Profession))
	{
		return TEXT("Invalid profession");
	}
	
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetWorld());
	if (Identity->GetLoginStatus(0) != ELoginStatus::LoggedIn)
	{
		return TEXT("Not logged in");
	}

	const auto Handle = CharacterServiceClient->InitCreateCharacter();
	auto MetaData = UTurboLinkGrpcUtilities::CreateAuthMetaData(Identity->GetAuthToken(0));
	
	FGrpcSroCharacterCreateCharacterRequest Request;
	Request.OwnerId = Identity->GetUniquePlayerId(0)->ToString();
	Request.Name = Name;
	Request.Realm = Realm;
	Request.Gender = Gender;
	Request.DimensionId = DimensionId;
	Request.Profession = Profession;
	
	CharacterServiceClient->CreateCharacter(Handle, Request, MetaData);
	return TEXT("");
}

FString UCommonCharacterSubsystem::SearchAllCharacters()
{
	IOnlineIdentityPtr Identity = Online::GetIdentityInterface(GetWorld());
	if (Identity->GetLoginStatus(0) != ELoginStatus::LoggedIn)
	{
		return TEXT("Not logged in");
	}
	const FUniqueNetIdPtr UniqueNetId = Identity->GetUniquePlayerId(0);

	const auto Handle = CharacterServiceClient->InitGetCharactersForUser();
	FGrpcSroCharacterGetUserCharactersRequest Request;
	Request.OwnerId = UniqueNetId->ToString();
	auto MetaData = UTurboLinkGrpcUtilities::CreateAuthMetaData(Identity->GetAuthToken(0));
	CharacterServiceClient->GetCharactersForUser(Handle, Request, MetaData);
	
	return TEXT("");
}

void UCommonCharacterSubsystem::OnGetCharacterResponseReceived(FGrpcContextHandle Handle, const FGrpcResult& GrpcResult,
                                                               const FGrpcSroCharacterCharacter& Response)
{
}

void UCommonCharacterSubsystem::OnCreateCharacterResponseReceived(FGrpcContextHandle Handle,
                                                                  const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacter& Response)
{
	if (GrpcResult.Code != EGrpcResultCode::Ok)
	{
		OnCharacterCreatedError.Broadcast(GrpcResult.Message);
		return;
	}

	const int Index = AllCharacters.Add(Response);
	CurrentCharacter = AllCharacters[Index];
	OnAllCharactersChanged.Broadcast(AllCharacters, TEXT(""));
	OnCharacterCreated.Broadcast(CurrentCharacter);
}

void UCommonCharacterSubsystem::OnDeleteCharacterResponseReceived(FGrpcContextHandle Handle,
                                                                  const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)
{
	if (GrpcResult.Code != EGrpcResultCode::Ok)
	{
		OnCharacterDeletedError.Broadcast(GrpcResult.Message);
		return;
	}
	
	FGrpcSroCharacterCharacter Character = PendingDeleteCharacters.FindAndRemoveChecked(Handle);
	AllCharacters.RemoveAll([Character](const FGrpcSroCharacterCharacter& CurrentCharacter)
		{
			return CurrentCharacter.Id == Character.Id;
		});
	OnAllCharactersChanged.Broadcast(AllCharacters, TEXT(""));
}

void UCommonCharacterSubsystem::OnGetCharactersResponseReceived(FGrpcContextHandle Handle,
                                                                const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacters& Response)
{
}

void UCommonCharacterSubsystem::OnGetCharactersForUserResponseReceived(FGrpcContextHandle Handle,
	const FGrpcResult& GrpcResult, const FGrpcSroCharacterCharacters& Response)
{
	FString Error;
	if (GrpcResult.Code != EGrpcResultCode::Ok)
	{
		Error = TEXT("request failed");
		UE_LOG(LogCommonCharacter, Error, TEXT("GetAllCharactersForUserResponse (%d): %s"), GrpcResult.Code, *GrpcResult.Message)
	}
	else
	{
		AllCharacters = Response.Characters;
	}

	OnAllCharactersChanged.Broadcast(Response.Characters, Error);
}

bool UCommonCharacterSubsystem::IsValidGender(const FString& Value) const
{
	return Genders.Contains(Value.ToLower());
}

bool UCommonCharacterSubsystem::IsValidRealm(const FString& Value) const
{
	return Realms.Contains(Value.ToLower());	
}

bool UCommonCharacterSubsystem::IsValidProfession(const FString& Value) const
{
	return Professions.Contains(Value.ToLower());
}

FString UCommonCharacterSubsystem::IsValidName(const FString& Value) const
{
	if (Value.Len() < 3)
	{
		return TEXT("Name is too short");
	}
	
	if (Value.Len() >= 15)
	{
		return TEXT("Name is too long");
	}
	
	const FRegexPattern Pattern(TEXT("^[a-zA-Z0-9]+$"));
	FRegexMatcher Matcher(Pattern, Value);
	if (!Matcher.FindNext())
	{
		return TEXT("Name contains invalid characters");
	}
	
	return TEXT("");
}
