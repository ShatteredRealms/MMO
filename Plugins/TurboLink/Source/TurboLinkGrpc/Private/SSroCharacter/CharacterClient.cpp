//Generated by TurboLink CodeGenerator, do not edit!
#include "SSroCharacter/CharacterClient.h"
#include "SSroCharacter/CharacterService.h"
#include "CharacterContext.h"
#include "TurboLinkGrpcManager_Private.h"
#include "Templates/SharedPointer.h"

FGrpcContextHandle UCharacterServiceClient::InitGetCharacter()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_GetCharacter>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::GetCharacter(FGrpcContextHandle Handle, const FGrpcSroCharacterGetCharacterRequest& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextGetCharacter = StaticCastSharedPtr<GrpcContext_CharacterService_GetCharacter>(*context);
		contextGetCharacter->Call(Request);
	}
}

FGrpcContextHandle UCharacterServiceClient::InitGetCharacters()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_GetCharacters>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::GetCharacters(FGrpcContextHandle Handle, const FGrpcSroCharacterGetCharactersRequest& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextGetCharacters = StaticCastSharedPtr<GrpcContext_CharacterService_GetCharacters>(*context);
		contextGetCharacters->Call(Request);
	}
}

FGrpcContextHandle UCharacterServiceClient::InitGetCharactersForUser()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_GetCharactersForUser>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::GetCharactersForUser(FGrpcContextHandle Handle, const FGrpcSroCharacterGetUserCharactersRequest& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextGetCharactersForUser = StaticCastSharedPtr<GrpcContext_CharacterService_GetCharactersForUser>(*context);
		contextGetCharactersForUser->Call(Request);
	}
}

FGrpcContextHandle UCharacterServiceClient::InitCreateCharacter()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_CreateCharacter>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::CreateCharacter(FGrpcContextHandle Handle, const FGrpcSroCharacterCreateCharacterRequest& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextCreateCharacter = StaticCastSharedPtr<GrpcContext_CharacterService_CreateCharacter>(*context);
		contextCreateCharacter->Call(Request);
	}
}

FGrpcContextHandle UCharacterServiceClient::InitDeleteCharacter()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_DeleteCharacter>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::DeleteCharacter(FGrpcContextHandle Handle, const FGrpcSroTargetId& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextDeleteCharacter = StaticCastSharedPtr<GrpcContext_CharacterService_DeleteCharacter>(*context);
		contextDeleteCharacter->Call(Request);
	}
}

FGrpcContextHandle UCharacterServiceClient::InitEditCharacter()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_EditCharacter>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::EditCharacter(FGrpcContextHandle Handle, const FGrpcSroCharacterEditCharacterRequest& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextEditCharacter = StaticCastSharedPtr<GrpcContext_CharacterService_EditCharacter>(*context);
		contextEditCharacter->Call(Request);
	}
}

FGrpcContextHandle UCharacterServiceClient::InitAddCharacterPlayTime()
{
	FGrpcContextHandle handle = Service->TurboLinkManager->GetNextContextHandle();
	auto context = UGrpcClient::MakeContext<GrpcContext_CharacterService_AddCharacterPlayTime>(handle);
	context->RpcContext = UTurboLinkGrpcManager::Private::CreateRpcClientContext();
	return context->GetHandle();
}

void UCharacterServiceClient::AddCharacterPlayTime(FGrpcContextHandle Handle, const FGrpcSroCharacterAddPlayTimeRequest& Request, TMap<FString, FString>& MetaData)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		for (auto const& Pair : MetaData)
		{
			context->Get()->RpcContext->AddMetadata(TCHAR_TO_UTF8(*Pair.Key), TCHAR_TO_UTF8(*Pair.Value));
		}
		auto contextAddCharacterPlayTime = StaticCastSharedPtr<GrpcContext_CharacterService_AddCharacterPlayTime>(*context);
		contextAddCharacterPlayTime->Call(Request);
	}
}

void UCharacterServiceClient::TryCancel(FGrpcContextHandle Handle)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		(*context)->TryCancel();
	}
}

void UCharacterServiceClient::Shutdown()
{
	OnGetCharacterResponse.Clear();
	OnGetCharactersResponse.Clear();
	OnGetCharactersForUserResponse.Clear();
	OnCreateCharacterResponse.Clear();
	OnDeleteCharacterResponse.Clear();
	OnEditCharacterResponse.Clear();
	OnAddCharacterPlayTimeResponse.Clear();
	Super::Shutdown();
}

