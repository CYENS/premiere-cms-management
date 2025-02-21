
#pragma once

#include "CoreMinimal.h"
#include "Repositories/UserRepository.h"
#include "Structs/CMSTypes.h"
#include "Structs/CMSInputs.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PremiereCMSManagementSubsystem.generated.h"

struct FCMSPerformanceWhereUniqueInput;
class UUsdSceneRepository;
class UGraphQLDataSource;
class USessionRepository;
class UUserRepository;
class UPerformanceRepository;
enum class EGQLSessionState : uint8;

class UPremiereCMSDeveloperSettings;

struct FCMSPerformanceCreateInput;

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetSession, const FCMSSession&, Session);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetSessions, const TArray<FCMSSession>&, Sessions);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUserSuccess, FCMSUser, User);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateUserSuccess, FCMSUser, User);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreatePerformanceSuccess, FCMSPerformance, Performance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetPerformanceSuccess, FCMSPerformance, Performance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllUsersSuccess, const TArray<FCMSUser>&, Users);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllUsdScenesSuccess, const TArray<FCMSUsdScene>&, UsdScenes);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUsdSceneSuccess, const FCMSUsdScene&, UsdScene);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllPerformancesSuccess, const TArray<FCMSPerformance>&, Performances);
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
	
	UPROPERTY()
	const UPremiereCMSDeveloperSettings* DeveloperSettings;
	
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
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | User")
	void FindUser(
		const FCMSIdInput& Where,
		const FOnGetUserSuccess& OnFindUserSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | User")
	void CreateUser(
		const FCMSUserCreateInput& Data,
		const FString& PersonId,
		const FOnGetUserSuccess& OnCreateUserSuccess,
		const FOnFailureDelegate& OnFailure
	);

	/* Performance */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void GetAllPerformances(
		FOnGetAllPerformancesSuccess OnGetAllPerformancesSuccess,
		FOnFailureDelegate OnGetAllPerformanceFailure
	) const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void CreatePerformance(
		const FCMSPerformanceCreateInput& CreatePerformanceInput,
		const FString& OwnerId,
		const FOnCreatePerformanceSuccess& OnCreatePerformanceSuccess,
		const FOnFailureDelegate& OnFailure
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
		const FOnGetPerformanceSuccess& OnUserAddSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveUserFromPerformance(
		const FCMSUserPerformanceWhereUniqueInput& Where,
		FOnGetPerformanceSuccess OnUserRemoveSuccess,
		FOnFailureDelegate OnFailure
	);

	/* Session */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void GetAllSessions(
		const FOnGetSessions& OnGetAllSessionsSuccess,
		const FOnFailureDelegate& OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void FindSession(
		const FCMSSessionWhereUniqueInput& Where,
		const FOnGetSession& OnGetSessionSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void GetActiveSessions(
		const FOnGetSessions& OnGetActiveSessions,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session", meta=(AutoCreateRefTerm="FaceDataWhereIds,AudioDataWhereIds"))
	void CreateSession(
		const FCMSSessionCreateInput& Data,
		const FString& OwnerWhereId,
		const EGQLSessionState SessionState,
		const TArray<FString>& AudioDataWhereIds,
		const TArray<FString>& FaceDataWhereIds,
		const FOnGetSession& OnCreateSessionSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	void GetAllSessions(
		const FOnGetSessions& OnGetAllPerformancesSuccess,
		const FOnFailureDelegate& OnFailure
	) const;

	/* UsdScenes */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void GetAllUsdScenes(
		FOnGetAllUsdScenesSuccess OnGetAllUsdScenesSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void CreateUsdScene(
		const FCMSUsdSceneCreateInput& UsdSceneCreateInput,
		const FString& OwnerId,
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
