//Generated by TurboLink CodeGenerator, do not edit!
#pragma once
#include "TurboLinkGrpcMessage.h"
#include "SGoogleApi/AnnotationsMessage.h"
#include "SGoogleProtobuf/FieldMaskMessage.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryMessage.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName="SroCharacter.GetInventoryRequest"))
struct FGrpcSroCharacterGetInventoryRequest : public FGrpcMessage
{
    GENERATED_BODY()
    DECLARE_JSON_FUNCTIONS()

    UPROPERTY(BlueprintReadWrite, Category = TurboLink)
    FGrpcGoogleProtobufFieldMask Mask;

    UPROPERTY(BlueprintReadWrite, Category = TurboLink)
    FString CharacterId = "";
};

USTRUCT(BlueprintType, meta = (DisplayName="SroCharacter.GetInventoryResponse",
    HasNativeMake = "TurboLinkGrpc.GrpcSroCharacterGetInventoryResponseHelperLibrary.MakeGetInventoryResponse",
    HasNativeBreak = "TurboLinkGrpc.GrpcSroCharacterGetInventoryResponseHelperLibrary.BreakGetInventoryResponse"))
struct FGrpcSroCharacterGetInventoryResponse : public FGrpcMessage
{
    GENERATED_BODY()
    DECLARE_JSON_FUNCTIONS()

    TArray<TSharedPtr<FGrpcSroCharacterItem>> Items;
};

USTRUCT(BlueprintType, meta = (DisplayName="SroCharacter.Item"))
struct FGrpcSroCharacterItem : public FGrpcMessage
{
    GENERATED_BODY()
    DECLARE_JSON_FUNCTIONS()

    UPROPERTY(BlueprintReadWrite, Category = TurboLink)
    FUInt32 Id = 0;

    UPROPERTY(BlueprintReadWrite, Category = TurboLink)
    FUInt32 Slot = 0;

    UPROPERTY(BlueprintReadWrite, Category = TurboLink)
    FUInt32 Quantity = 0;
};

UCLASS()
class UGrpcSroCharacterGetInventoryResponseHelperLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintPure, Category = "TurboLink|SroCharacter", meta=(BlueprintThreadSafe))
    static FGrpcSroCharacterGetInventoryResponse MakeGetInventoryResponse( 
        TArray<FGrpcSroCharacterItem> Items 
    );

    UFUNCTION(BlueprintPure, Category = "TurboLink|SroCharacter", meta=(BlueprintThreadSafe))
    static void BreakGetInventoryResponse(const FGrpcSroCharacterGetInventoryResponse& GetInventoryResponse,  
        TArray<FGrpcSroCharacterItem>& Items 
    );
};
