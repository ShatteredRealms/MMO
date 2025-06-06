//Generated by TurboLink CodeGenerator, do not edit!
#pragma once
#include "TurboLinkGrpcService.h"
#include "SSroGameserver/ConnectionClient.h"
#include "ConnectionService.generated.h"

class UTurboLinkGrpcManager;

UCLASS(ClassGroup = TurboLink, BlueprintType)
class TURBOLINKGRPC_API UConnectionService : public UGrpcService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = TurboLink)
	virtual void Connect() override;
	
	UFUNCTION(BlueprintCallable, Category = TurboLink)
	virtual EGrpcServiceState GetServiceState() const override;

	UFUNCTION(BlueprintCallable, Category = TurboLink)
	UConnectionServiceClient* MakeClient();

private:
	virtual void Shutdown() override;

	UPROPERTY()
	TObjectPtr<UConnectionServiceClient> InnerClient; 
public:
	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroGameserverConnectGameServerResponse& Response)> FConnectGameServerResponseLambda;
	void CallConnectGameServer(const FGrpcSroTargetId& Request, FConnectGameServerResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroTargetId& Response)> FVerifyConnectResponseLambda;
	void CallVerifyConnect(const FGrpcSroGameserverVerifyConnectRequest& Request, FVerifyConnectResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroGameserverConnectGameServerResponse& Response)> FTransferPlayerResponseLambda;
	void CallTransferPlayer(const FGrpcSroGameserverTransferPlayerRequest& Request, FTransferPlayerResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroGameserverConnectionStatus& Response)> FIsCharacterPlayingResponseLambda;
	void CallIsCharacterPlaying(const FGrpcSroTargetId& Request, FIsCharacterPlayingResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroGameserverConnectionStatus& Response)> FIsUserPlayingResponseLambda;
	void CallIsUserPlaying(const FGrpcSroTargetId& Request, FIsUserPlayingResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

public:
	class Private;
	Private* const d = nullptr;

public:
	UConnectionService();
	virtual ~UConnectionService();
};

