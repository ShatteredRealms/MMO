//Generated by TurboLink CodeGenerator, do not edit!
#pragma once
#include "TurboLinkGrpcService.h"
#include "SSro/HealthClient.h"
#include "HealthService.generated.h"

class UTurboLinkGrpcManager;

UCLASS(ClassGroup = TurboLink, BlueprintType)
class TURBOLINKGRPC_API UHealthService : public UGrpcService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = TurboLink)
	virtual void Connect() override;
	
	UFUNCTION(BlueprintCallable, Category = TurboLink)
	virtual EGrpcServiceState GetServiceState() const override;

	UFUNCTION(BlueprintCallable, Category = TurboLink)
	UHealthServiceClient* MakeClient();

private:
	virtual void Shutdown() override;

	UPROPERTY()
	TObjectPtr<UHealthServiceClient> InnerClient; 
public:
	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroHealthMessage& Response)> FHealthResponseLambda;
	void CallHealth(const FGrpcGoogleProtobufEmpty& Request, FHealthResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

public:
	class Private;
	Private* const d = nullptr;

public:
	UHealthService();
	virtual ~UHealthService();
};

