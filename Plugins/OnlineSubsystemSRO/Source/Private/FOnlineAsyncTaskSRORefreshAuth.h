#pragma once
#include "OnlineAsyncTaskManager.h"
#include "OnlineSubsystemSRO.h"
#include "AuthTypes.h"

struct FOnlineAsyncTaskSRORefreshAuth
	: public FOnlineAsyncTaskBasic<FOnlineSubsystemSRO>
{
public:
	FOnlineAsyncTaskSRORefreshAuth(
			FOnlineSubsystemSRO* InSubsystem,
			int32 InLocalUserNum)
		: FOnlineAsyncTaskBasic(InSubsystem)
		, bRequestMade(false)
		, LocalUserNum(InLocalUserNum)
	{
	}
	
	virtual void Finalize() override;
	virtual void Initialize() override;
	virtual void Tick() override;
	virtual void TriggerDelegates() override;
	virtual FString ToString() const override;

protected:
	bool bRequestMade;
	int32 LocalUserNum;
	FAuthTokenSRO AuthTokenSRO;

private:
	FTimespan RefreshBufferTime = ETimespan::TicksPerMinute * 4;
};
