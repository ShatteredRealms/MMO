//Generated by TurboLink CodeGenerator, do not edit!
#pragma once
#include "TurboLinkGrpcService.h"
#include "SSroChat/ChatClient.h"
#include "ChatService.generated.h"

class UTurboLinkGrpcManager;

UCLASS(ClassGroup = TurboLink, BlueprintType)
class TURBOLINKGRPC_API UChatService : public UGrpcService
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = TurboLink)
	virtual void Connect() override;
	
	UFUNCTION(BlueprintCallable, Category = TurboLink)
	virtual EGrpcServiceState GetServiceState() const override;

	UFUNCTION(BlueprintCallable, Category = TurboLink)
	UChatServiceClient* MakeClient();

private:
	virtual void Shutdown() override;

	UPROPERTY()
	TObjectPtr<UChatServiceClient> InnerClient; 
public:
	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatMessage& Response)> FConnectChatChannelResponseLambda;
	typedef TFunction<void()> FConnectChatChannelFinishLambda;
	void CallConnectChatChannel(const FGrpcSroChatConnectChatChannelRequest& Request, FConnectChatChannelResponseLambda ResponseLambda, FConnectChatChannelFinishLambda FinishLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatMessage& Response)> FConnectDirectMessagesResponseLambda;
	typedef TFunction<void()> FConnectDirectMessagesFinishLambda;
	void CallConnectDirectMessages(const FGrpcSroTargetId& Request, FConnectDirectMessagesResponseLambda ResponseLambda, FConnectDirectMessagesFinishLambda FinishLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)> FSendChatChannelMessageResponseLambda;
	void CallSendChatChannelMessage(const FGrpcSroChatSendChatChannelMessageRequest& Request, FSendChatChannelMessageResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)> FSendDirectMessageResponseLambda;
	void CallSendDirectMessage(const FGrpcSroChatSendDirectMessageRequest& Request, FSendDirectMessageResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatChannels& Response)> FGetChatChannelsResponseLambda;
	void CallGetChatChannels(const FGrpcGoogleProtobufEmpty& Request, FGetChatChannelsResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatChannel& Response)> FGetChatChannelResponseLambda;
	void CallGetChatChannel(const FGrpcSroTargetId& Request, FGetChatChannelResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatChannel& Response)> FCreateChatChannelResponseLambda;
	void CallCreateChatChannel(const FGrpcSroChatCreateChatChannelMessage& Request, FCreateChatChannelResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatChannel& Response)> FDeleteChatChannelResponseLambda;
	void CallDeleteChatChannel(const FGrpcSroTargetId& Request, FDeleteChatChannelResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatChannel& Response)> FEditChatChannelResponseLambda;
	void CallEditChatChannel(const FGrpcSroChatUpdateChatChannelRequest& Request, FEditChatChannelResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatChannels& Response)> FGetAuthorizedChatChannelsResponseLambda;
	void CallGetAuthorizedChatChannels(const FGrpcSroTargetId& Request, FGetAuthorizedChatChannelsResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)> FSetCharacterChatChannelAuthResponseLambda;
	void CallSetCharacterChatChannelAuth(const FGrpcSroChatRequestSetCharacterSetChatChannelAuth& Request, FSetCharacterChatChannelAuthResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)> FUpdateCharacterChatChannelAuthResponseLambda;
	void CallUpdateCharacterChatChannelAuth(const FGrpcSroChatRequestUpdateCharacterSetChatChannelAuth& Request, FUpdateCharacterChatChannelAuthResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcGoogleProtobufEmpty& Response)> FBanCharacterFromChatChannelResponseLambda;
	void CallBanCharacterFromChatChannel(const FGrpcSroChatBanRequest& Request, FBanCharacterFromChatChannelResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

	typedef TFunction<void(const FGrpcResult& GrpcResult, const FGrpcSroChatChatLogs& Response)> FGetChatLogsResponseLambda;
	void CallGetChatLogs(const FGrpcSroChatChatLogRequest& Request, FGetChatLogsResponseLambda ResponseLambda, TMap<FString, FString>& MetaData);

public:
	class Private;
	Private* const d = nullptr;

public:
	UChatService();
	virtual ~UChatService();
};

