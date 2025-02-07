
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
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUsdSceneSuccess, const FCMSUsdScene&, UsdScene);
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
	
	/* User */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | User")
	void GetAllUsers(
		FOnGetAllUsersSuccess OnGetAllUsersSuccess,
		FOnFailureDelegate OnGetAllUsersFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void CreateUser(
		const FCMSUser& User,
		FOnCreateUserSuccess OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);

	/* Performance */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void GetAllPerformances(
		FOnGetAllPerformancesSuccess OnGetAllPerformancesSuccess,
		FOnFailureDelegate OnGetAllPerformanceFailure
	) const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void CreatePerformance(
		FCMSPerformanceCreateInput CreatePerformanceInput,
		FOnCreatePerformanceSuccess OnCreatePerformanceSuccess,
		FOnFailureDelegate OnCreatePerformanceFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void FindPerformance(
		const FCMSPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnGetPerformanceSuccess,
		FOnFailureDelegate OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void DeletePerformance(
		const FCMSPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnGetPerformanceSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void UpdatePerformance(
		const FCMSPerformanceWhereUniqueInput& Where,
		const FCMSPerformanceUpdateInput& Data,
		FOnGetPerformanceSuccess OnGetPerformanceSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void AddUsdSceneToPerformance(
		const FCMSUsdScenePerformanceWhereInput& Where,
	    FOnGetPerformanceSuccess OnUsdSceneAddSuccess,
	    FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveUsdSceneFromPerformance(
		const FCMSUsdScenePerformanceWhereInput& Where,
	    FOnGetPerformanceSuccess OnUsdSceneRemoveSuccess,
	    FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void AddUserToPerformance(
		const FCMSUserPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnUserAddSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveUserFromPerformance(
		const FCMSUserPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnUserRemoveSuccess,
		FOnFailureDelegate OnFailure
	);

	/* Session */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void GetActiveSessions(
		FOnGetActiveSessionsDelegate OnGetActiveSessions,
		FOnFailureDelegate OnGetActiveSessionsFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void CreateSession(
		FCMSSession Session,
		FOnCreateSessionSuccessDelegate OnCreateSessionSuccess,
		FOnFailureDelegate OnCreateSessionFailure
	);
	
	/* UsdScenes */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void GetAllUsdScenes(
		FOnGetAllUsdScenesSuccess OnGetAllUsdScenesSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void CreateUsdScene(
		const FCMSUsdSceneCreateInput& UsdSceneCreateInput,
		FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void UpdateUsdScene(
		const FCMSUsdSceneWhereUniqueInput& Where,
		const FCMSUsdSceneUpdateInput& Data,
		FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void FindUsdScene(
		const FCMSUsdSceneWhereUniqueInput& Where,
		FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
	    FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void DeleteUsdScene(
		const FCMSUsdSceneWhereUniqueInput& Where,
		FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
		FOnFailureDelegate OnFailure
	);
	
};
