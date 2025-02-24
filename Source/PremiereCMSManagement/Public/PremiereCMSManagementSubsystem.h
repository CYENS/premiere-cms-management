
#pragma once

#include "CoreMinimal.h"
#include "Repositories/AvatarRepository.h"
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
class UPersonRepository;
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
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAvatar, const FCMSAvatar&, Avatar);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAvatars, const TArray<FCMSAvatar>&, Avatars);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetPerson, const FCMSPerson&, Person);

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
	UPersonRepository* PersonRepository;
	
	UPROPERTY()
	const UPremiereCMSDeveloperSettings* DeveloperSettings;
	
	UPROPERTY()
	UAvatarRepository* AvatarRepository;

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
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | User")
	void AddPersonToUser(
		const FCMSUserWhereUniqueInput& UserWhere,
		const FCMSPersonWhereUniqueInput& PersonWhere,
		const FOnGetUserSuccess& OnAddPersonToUserSuccess,
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
	void AddSessionToPerformance(
		const FCMSIdInput& PerformanceWhere,
		const FCMSIdInput& SessionWhere,
		const FOnGetPerformanceSuccess& OnUsdSceneAddSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveSessionFromPerformance(
		const FCMSIdInput& PerformanceWhere, const FCMSIdInput& SessionWhere,
	    const FOnGetPerformanceSuccess& OnUsdSceneRemoveSuccess,
	    const FOnFailureDelegate& OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveUsdSceneFromPerformance(
		const FCMSUsdScenePerformanceWhereInput& Where,
	    FOnGetPerformanceSuccess OnUsdSceneRemoveSuccess,
	    FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void AddUserToPerformance(
		const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
		const FCMSUserWhereUniqueInput& UserWhere,
		const FOnGetPerformanceSuccess& OnUserAddSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveUserFromPerformance(
		const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
		const FCMSUserWhereUniqueInput& UserWhere,
		const FOnGetPerformanceSuccess& OnUserRemoveSuccess,
		const FOnFailureDelegate& OnFailure
	);

	/* Avatars */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void AddAvatarToPerformance(
		const FCMSIdInput& PerformanceWhere,
		const FCMSIdInput& AvatarWhere,
		const FOnGetPerformanceSuccess& OnAvatarAddSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveAvatarFromPerformance(
		const FCMSIdInput& PerformanceWhere,
		const FCMSIdInput& AvatarWhere,
	    const FOnGetPerformanceSuccess& OnUsdSceneRemoveSuccess,
	    const FOnFailureDelegate& OnFailure
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
		const FString& UsdSceneWhereId,
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
	
	/*  Avatar */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdScene")
	void CreateAvatar(
		const FCMSAvatarCreateInput& Data,
		const FString& PerformanceId,
		FOnGetAvatar OnCreateAvatarSuccess,
		FOnFailureDelegate OnFailure
	);
	
	/* Person */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Person")
	void CreatePerson(
		const FCMSPersonCreateInput& Data,
		const FString& UserId,
		FOnGetPerson OnCreatePersonSuccess,
		FOnFailureDelegate OnFailure
	);
};
