
#pragma once

#include "CoreMinimal.h"
#include "GraphQLDataSource.h"
#include "Repositories/SessionRepository.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PremiereCMSManagementSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateSessionSuccessDelegate, FCMSSession, Session);
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
	void CreateSession(
		const FString& Title,
		const FString& OwnerId,
		const FString& PerformanceId,
		const FString& State,
		FOnCreateSessionSuccessDelegate OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);
};
