
#pragma once

#include "CoreMinimal.h"
#include "Structs/CMSSession.h"
#include "Structs/CMSUser.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PremiereCMSManagementSubsystem.generated.h"

class UGraphQLDataSource;
class UUserRepository;
class USessionRepository;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateSessionSuccessDelegate, FCMSSession, Session);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateUserSuccess, FCMSUser, User);
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
	
	UPROPERTY()
	UUserRepository* UserRepository;
	
public:
	UPROPERTY(BlueprintReadOnly, Config, Category="PremiereCMSManagement | Settings")
	FString GraphQLUrl;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQueryFString() const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQueryFVariant() const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQueryFJsonValue() const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void CreateSession(
		FCMSSession Session,
		FOnCreateSessionSuccessDelegate OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void CreateUser(
		const FCMSUser& User,
		FOnCreateUserSuccess OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void GetActiveSessions(
		FOnGetActiveSessionsDelegate OnGetActiveSessions,
		FOnFailureDelegate OnGetActiveSessionsFailure
	);
};
