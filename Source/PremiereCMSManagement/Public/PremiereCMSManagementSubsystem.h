
#pragma once

#include "CoreMinimal.h"
#include "GraphQLDataSource.h"
#include "Repositories/SessionRepository.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PremiereCMSManagementSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateSessionSuccessDelegate, FCMSSession, Session);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetActiveSessionsDelegate, const TArray<FCMSSession>&, Sessions);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFailureDelegate, FString, ErrorMessage);

UCLASS(BlueprintType, Config=Engine)
class PREMIERECMSMANAGEMENT_API UPremiereCMSManagementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UPremiereCMSManagementSubsystem();

	UPROPERTY()
	UGraphQLDataSource* GraphQlDataSource;
	
	UPROPERTY()
	USessionRepository* SessionRepository;
	
	
public:
	UPROPERTY(BlueprintReadOnly, Config, Category="PremiereCMSManagement | Settings")
	FString GraphQLUrl;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQuery();
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void CreateSession(
		FCMSSession Session,
		FOnCreateSessionSuccessDelegate OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void GetActiveSessions(
		FOnGetActiveSessionsDelegate OnGetActiveSessions,
		FOnFailureDelegate OnGetActiveSessionsFailure
	);
};
