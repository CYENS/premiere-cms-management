
#pragma once

#include "CoreMinimal.h"
#include "Structs/CMSSession.h"
#include "Structs/CMSTypes.h"
#include "Structs/CMSInputs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PremiereCMSManagementSubsystem.generated.h"

class UUsdSceneRepository;
struct FCMSPerformanceWhereUniqueInput;
class UGraphQLDataSource;
class USessionRepository;
class UUserRepository;
class UPerformanceRepository;

struct FCMSPerformanceCreateInput;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateSessionSuccessDelegate, FCMSSession, Session);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateUserSuccess, FCMSUser, User);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreatePerformanceSuccess, FCMSPerformance, Performance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetPerformanceSuccess, FCMSPerformance, Performance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllUsersSuccess, const TArray<FCMSUser>&, Users);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllUsdScenesSuccess, const TArray<FCMSUsdScene>&, UsdScenes);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllPerformancesSuccess, const TArray<FCMSPerformance>&, Performances);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetActiveSessionsDelegate, const TArray<FCMSSession>&, Sessions);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFailureDelegate, const FString&, ErrorMessage);

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
	
	UPROPERTY()
	UPerformanceRepository* PerformanceRepository;
	
	UPROPERTY()
	UUsdSceneRepository* UsdSceneRepository;
	
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
	void CreatePerformance(
		FCMSPerformanceCreateInput CreatePerformanceInput,
		FOnCreatePerformanceSuccess OnCreatePerformanceSuccess,
		FOnFailureDelegate OnCreatePerformanceFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void CreateUser(
		const FCMSUser& User,
		FOnCreateUserSuccess OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);

	/* Performances */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void GetAllPerformances(
		FOnGetAllPerformancesSuccess OnGetAllPerformancesSuccess,
		FOnFailureDelegate OnGetAllPerformanceFailure
	) const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void FindPerformance(
		const FCMSPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnGetPerformanceSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void AddUsdSceneToPerformance(
		const FCMSUsdScenePerformanceWhereInput& Where,
	    FOnGetPerformanceSuccess OnUsdSceneAddSuccess,
	    FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void RemoveUsdSceneFromPerformance(
		const FCMSUsdScenePerformanceWhereInput& Where,
	    FOnGetPerformanceSuccess OnUsdSceneRemoveSuccess,
	    FOnFailureDelegate OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void DeletePerformance(
		const FCMSPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnGetPerformanceSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void UpdatePerformance(
		const FCMSPerformanceWhereUniqueInput& Where,
		const FCMSPerformanceUpdateInput& Data,
		FOnGetPerformanceSuccess OnGetPerformanceSuccess,
		FOnFailureDelegate OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void GetAllUsers(
		FOnGetAllUsersSuccess OnGetAllUsersSuccess,
		FOnFailureDelegate OnGetAllUsersFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void GetActiveSessions(
		FOnGetActiveSessionsDelegate OnGetActiveSessions,
		FOnFailureDelegate OnGetActiveSessionsFailure
	);
	
	/* Performances */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void GetAllUsdScenes(
		FOnGetAllUsdScenesSuccess OnGetAllUsdScenesSuccess,
		FOnFailureDelegate OnFailure
	) const;
};
