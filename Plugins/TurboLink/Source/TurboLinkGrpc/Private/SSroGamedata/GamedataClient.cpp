//Generated by TurboLink CodeGenerator, do not edit!
#include "SSroGamedata/GamedataClient.h"
#include "SSroGamedata/GamedataService.h"
#include "GamedataContext.h"
#include "TurboLinkGrpcManager_Private.h"
#include "Templates/SharedPointer.h"

void UGameDataServiceClient::TryCancel(FGrpcContextHandle Handle)
{
	auto context = UGrpcClient::GetContext(Handle);
	if (context != nullptr)
	{
		(*context)->TryCancel();
	}
}

void UGameDataServiceClient::Shutdown()
{
	Super::Shutdown();
}

