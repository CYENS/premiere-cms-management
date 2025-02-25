#include "Async/AsyncFindPerson.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAsyncFindPerson* UAsyncFindPerson::AsyncFindPerson(UObject* WorldContextObject, const FCMSPersonWhereUniqueInput& Where)
{
	UAsyncFindPerson* AsyncNode = NewObject<UAsyncFindPerson>();
	AsyncNode->WorldContextObject = WorldContextObject;
	AsyncNode->Where = Where;
	return AsyncNode;
}

void UAsyncFindPerson::Activate()
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

	FOnGetPerson OnFindPerson;
	OnFindPerson.BindDynamic(this, &ThisClass::HandleSuccess);
	
	FOnFailureDelegate OnFailureDelegate;
	OnFailureDelegate.BindDynamic(this, &ThisClass::HandleFailure);
	
	// CMSSubsystem->FindPerson(
	// 	Where,
	// 	OnFindPerson,
	// 	OnFailureDelegate
	// );
}

void UAsyncFindPerson::HandleSuccess(const FCMSPerson& Person)
{
	const FString EmptyErrorMessage = TEXT("");
	if (!Person.Id.IsEmpty())
	{
		OnNotFound.Broadcast(Person, EmptyErrorMessage);
	}
	else
	{
		OnFindSuccess.Broadcast(Person, EmptyErrorMessage);
	}
	SetReadyToDestroy(); 
}

void UAsyncFindPerson::HandleFailure(const FString& ErrorMessage)
{
	const FCMSPerson EmptyPerson {}; 
	OnFailure.Broadcast(EmptyPerson, ErrorMessage);
	SetReadyToDestroy();
}
