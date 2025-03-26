#include "Async/AsyncCMSLogin.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAsyncCMSLogin* UAsyncCMSLogin::AsyncLogin(UObject* WorldContextObject)
{
	UAsyncCMSLogin* AsyncNode = NewObject<UAsyncCMSLogin>();
	AsyncNode->WorldContextObject = WorldContextObject;
	return AsyncNode;
}

void UAsyncCMSLogin::Activate()
{
	if (!WorldContextObject)
	{
		HandleFailure(TEXT("Invalid World Context Object"));
		return;
	}

	UPremiereCMSManagementSubsystem* CMSSubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UPremiereCMSManagementSubsystem>();
	if (!CMSSubsystem)
	{
		HandleFailure(TEXT("Failed to get CMS Subsystem"));
		return;
	}

	FOnLoginSuccess OnLoginSuccess;
	OnLoginSuccess.BindDynamic(this, &UAsyncCMSLogin::HandleSuccess);
	
	FOnFailureDelegate OnLoginFailure;
	OnLoginFailure.BindDynamic(this, &UAsyncCMSLogin::HandleFailure);
	
	CMSSubsystem->Login(
		OnLoginSuccess,
		OnLoginFailure
	);
}

void UAsyncCMSLogin::HandleSuccess()
{
	const FString EmptyErrorMessage = TEXT("");
	OnSuccess.Broadcast(EmptyErrorMessage);
	SetReadyToDestroy(); 
}

void UAsyncCMSLogin::HandleFailure(const FString& ErrorMessage)
{
	OnFailure.Broadcast(ErrorMessage);
	SetReadyToDestroy();
}
