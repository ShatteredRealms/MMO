//Generated by TurboLink CodeGenerator, do not edit!
#pragma once
#include "TurboLinkGrpcMessage.h"
#include "FieldMaskMessage.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName="GoogleProtobuf.FieldMask"))
struct FGrpcGoogleProtobufFieldMask : public FGrpcMessage
{
    GENERATED_BODY()
    DECLARE_JSON_FUNCTIONS()

    UPROPERTY(BlueprintReadWrite, Category = TurboLink)
    TArray<FString> Paths;
};
