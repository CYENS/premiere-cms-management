#include "Async/AsyncGetAllSessions.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAsyncGetAllSessions* UAsyncGetAllSessions::AsyncGetAllSessions(UObject* WorldContextObject)
{
	UAsyncGetAllSessions* AsyncNode = NewObject<UAsyncGetAllSessions>();
	AsyncNode->WorldContextObject = WorldContextObject;
	return AsyncNode;
}

void UAsyncGetAllSessions::Activate()
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

	FOnGetSessions OnGetSession;
	OnGetSession.BindDynamic(this, &ThisClass::HandleSuccess);
	
	FOnFailureDelegate OnFailureDelegate;
	OnFailureDelegate.BindDynamic(this, &ThisClass::HandleFailure);
	
	CMSSubsystem->GetAllSessions(
		OnGetSession,
		OnFailureDelegate
	);
}

void UAsyncGetAllSessions::HandleSuccess(const TArray<FCMSSession>& Sessions)
{
	const FString EmptyErrorMessage = TEXT("");
	OnSuccess.Broadcast(Sessions, EmptyErrorMessage);
	SetReadyToDestroy(); 
}

void UAsyncGetAllSessions::HandleFailure(const FString& ErrorMessage)
{
	const TArray<FCMSSession> EmptySessionsArray;
	OnFailure.Broadcast(EmptySessionsArray, ErrorMessage);
	SetReadyToDestroy();
}
