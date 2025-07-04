
#pragma once

#include "CoreMinimal.h"
#include "Repositories/AudioDataRepository.h"
#include "Repositories/AvatarMotionDataRepository.h"
#include "Repositories/AvatarRepository.h"
#include "Repositories/FileRepository.h"
#include "Repositories/UsdAssetRepositoryLibrary.h"
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
class USessionCastRepository;
enum class EGQLSessionState : uint8;
enum ERelateToOneCategoriesForSession : uint8;
enum ERelateToManyCategoriesForSession : uint8;

class UPremiereCMSDeveloperSettings;

struct FCMSPerformanceCreateInput;

USTRUCT(BlueprintType)
struct FCMSGraphQLResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EGraphQLOutcome> GraphQLOutcome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RawResponse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HttpStatusCode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ErrorMessage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> GraphQLErrors ;
};

DECLARE_DYNAMIC_DELEGATE(FOnLoginSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetSession, const FCMSSession&, Session);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetSessions, const TArray<FCMSSession>&, Sessions);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUserSuccess, FCMSUser, User);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreateUserSuccess, FCMSUser, User);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnCreatePerformanceSuccess, FCMSPerformance, Performance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetPerformanceSuccess, FCMSPerformance, Performance);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllUsersSuccess, const TArray<FCMSUser>&, Users);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllUsdScenesSuccess, const TArray<FCMSUsdScene>&, UsdScenes);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAvatarMotionDataSuccess, const FCMSAvatarMotionData, AvatarMotionData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAvatarMotionDatasSuccess, const TArray<FCMSAvatarMotionData>&, AvatarMotionDatas);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAudioDataSuccess, const FCMSAudioData, AvatarMotionData);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAudioDatasSuccess, const TArray<FCMSAudioData>&, AvatarMotionDatas);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUsdSceneSuccess, const FCMSUsdScene&, UsdScene);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAllPerformancesSuccess, const TArray<FCMSPerformance>&, Performances);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnFailureDelegate, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAvatar, const FCMSAvatar&, Avatar);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetAvatars, const TArray<FCMSAvatar>&, Avatars);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetPerson, const FCMSPerson&, Person);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetFile, const FCMSFile&, File);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetObjectWithFile, const FCMSObjectWithFile&, ObjectWithFile);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetSessionCast, const FCMSSessionCast&, SessionCast);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGraphQLResponseDelegate, const FCMSGraphQLResult&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUsdAssetLibrary, const FCMSUsdAssetLibrary&, UsdAssetLibrary);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGetUsdAssetLibraries, const TArray<FCMSUsdAssetLibrary>&, UsdAssetLibraries);

UENUM(BlueprintType)
enum ESessionVisibility : uint8
{
	Private,
	Public,
};

UCLASS(BlueprintType, Config=Engine)
class PREMIERECMSMANAGEMENT_API UPremiereCMSManagementSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

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
	
	UPROPERTY()
	UAvatarMotionDataRepository* AvatarMotionDataRepository;

	UPROPERTY()
	UAudioDataRepository* AudioDataRepository;
	
	UPROPERTY()
	UFileRepository* FileRepository;
	
	UPROPERTY()
	USessionCastRepository* SessionCastRepository;
	
	UPROPERTY()
	UUsdAssetLibraryRepository* UsdAssetLibrary;

public:
	UPROPERTY(BlueprintReadOnly, Config, Category="PremiereCMSManagement | Settings")
	FString GraphQLUrl;

	UFUNCTION(BlueprintCallable, Category = "PremiereCMSManagement | Settings")
	static FString GetDefaultOnlinePlatformService();

	UFUNCTION(BlueprintCallable, Category = "PremiereCMSManagement | Settings")
	static bool IsPlatformEOS();

	UFUNCTION(BlueprintCallable)
	void Login(const FOnLoginSuccess& OnSuccess, const FOnFailureDelegate& OnFailure);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQueryFString() const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQueryFVariant() const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement")
	void TestGraphQlQueryFJsonValue() const;
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | GraphQL")
	void ExecuteGraphQL(
		const FString Query,
		const FString Variables,
		const FOnGraphQLResponseDelegate& OnResponse
	);
	
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
	void FindUserByEosId(
		const FString& WhereEosId,
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
	void UpdateUser(
		const FCMSUserWhereUniqueInput& WhereUser,
		const FCMSUserUpdateInput& Data,
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
		const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
		const FCMSUsdSceneWhereUniqueInput& UsdSceneWhere,
		const FOnGetPerformanceSuccess& OnUsdSceneAddSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Performance")
	void RemoveUsdSceneFromPerformance(
		const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
		const FCMSUsdSceneWhereUniqueInput& UsdSceneWhere,
		const FOnGetPerformanceSuccess& OnUsdSceneRemovedSuccess,
		const FOnFailureDelegate& OnFailure
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

	/* AvatarMotionData */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | AvatarMotionData")
	void GetAllAvatarMotionDatas(
		const FOnGetAvatarMotionDatasSuccess& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | AvatarMotionData")
	void FindAvatarMotionData(
		const FString& WhereId,
		const FOnGetAvatarMotionDataSuccess& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | AvatarMotionData")
	void CreateAvatarMotionData(
		const FCMSAvatarMotionDataCreateInput& Data,
		const FOnGetAvatarMotionDataSuccess& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);

	/* AudioData */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | AudioData")
	void GetAllAudioData(
		const FOnGetAudioDatasSuccess& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | AudioData")
	void FindAudioData(
		const FString& WhereId,
		const FOnGetAudioDataSuccess& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | AvatarMotionData")
	void CreateAudioData(
		const FCMSAudioDataCreateInput& Data,
		const FOnGetAudioDataSuccess& OnSuccess,
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
	    const FOnGetPerformanceSuccess& OnSuccess,
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
	void DeleteSession(
		const FString& WhereId,
		const FOnGetSession& OnGetSessionSuccess,
		const FOnFailureDelegate& OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
    void FindSessionByEosSessionId(
        const FString& WhereEosSessionId,
		const FOnGetSession& OnFindSessionSuccess,
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
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session", meta=(AutoCreateRefTerm="Data,FaceDataWhereIds,AudioDataWhereIds"))
	void UpdateSession(
		const FCMSSessionWhereUniqueInput& Where,
		const FCMSSessionUpdateInput& Data,
		const FString& OwnerWhereId,
		const FString& UsdSceneWhereId,
		const EGQLSessionState SessionState,
		const TArray<FString>& AudioDataWhereIds,
		const TArray<FString>& FaceDataWhereIds,
		const FOnGetSession& OnCreateSessionSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session", meta=(AutoCreateRefTerm="Data,FaceDataWhereIds,AudioDataWhereIds"))
	void UpdateSessionEosId(
		const FString& WhereId,
		const FString& EosSessionId,
		const FOnGetSession& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void ConnectOneItemToSession(
		const FString& SessionWhereId,
		const FString& ItemWhereId,
		const ERelateToOneCategoriesForSession ItemToConnectType,
		const FOnGetSession& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void DisconnectOneItemFromSession(
		const FString& SessionWhereId,
		const ERelateToOneCategoriesForSession ItemToConnectType,
		const FOnGetSession& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void ConnectManyItemsToSession(
		const FString& SessionWhereId,
		const TArray<FString>& ItemWhereId,
		const ERelateToManyCategoriesForSession ItemToConnectType,
		const FOnGetSession& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void DisconnectManyItemsFromSession(
		const FString& SessionWhereId,
		const TArray<FString>& ItemWhereId,
		const ERelateToManyCategoriesForSession ItemToConnectType,
		const FOnGetSession& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
	void UpdateSessionStateByEosSessionId(
		const FString& WhereEosSessionId,
		EGQLSessionState SessionState,
		const FOnGetSession& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Session")
    void UpdateSessionVisibilityByEosSessionId(
        const FString& WhereEosSessionId,
        const ESessionVisibility NewSessionVisibility,
        const FOnGetSession& OnSuccess,
        const FOnFailureDelegate& OnFailure
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
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Avatar")
	void CreateAvatar(
		const FCMSAvatarCreateInput& Data,
		const FString& PerformanceId,
		FOnGetAvatar OnCreateAvatarSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Avatar")
	void FindAvatar(
		const FString& WhereId,
		const FOnGetAvatar& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);

	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Avatar")
	void UpdateAvatar(
		const FString WhereId,
		const FCMSAvatarUpdateInput& UpdateData,
		const FOnGetAvatar& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	
	/* Person */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Person")
	void CreatePerson(
		const FCMSPersonCreateInput& Data,
		const FString& UserId,
		FOnGetPerson OnCreatePersonSuccess,
		FOnFailureDelegate OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Person")
	void FindPerson(
		const FCMSPersonWhereUniqueInput& Where,
		const FOnGetPerson& OnFindPersonSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | Person")
	void FindPersonByGivenNameAndFamilyName(
		const FString& GivenName,
		const FString& FamilyName,
		const FOnGetPerson& OnFindPersonSuccess,
		const FOnFailureDelegate& OnFailure
	);

	/* File */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | File")
	void UploadFileToObject(
		const EGraphQLOperationType Operation,
		const FCMSIdInput& Where,
		const FString& FilePath,
		const FOnGetObjectWithFile& OnUploadSuccess,
		const FOnFailureDelegate& OnFailure
	);

	/* SessionCast */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | SessionCast")
	void CreateSessionCast(
		const FCMSAvatarWhereUniqueInput& AvatarWhere,
		const FCMSSessionWhereUniqueInput& SessionWhere,
		const FCMSUserWhereUniqueInput& UserWhere,
		const FOnGetSessionCast& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	/* UsdAssetLibrary */
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdAssetLibrary")
	void GetAllUsdAssetLibraries(
		const FOnGetUsdAssetLibraries& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdAssetLibrary")
	void FindUsdAssetLibrary(
		FString WhereId,
		const FOnGetUsdAssetLibrary& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdAssetLibrary")
	void CreateUsdAssetLibrary(
		FCMSUsdAssetLibraryCreateInput CreateData,
		const FOnGetUsdAssetLibrary& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
	
	UFUNCTION(BlueprintCallable, Category="PremiereCMSManagement | UsdAssetLibrary")
	void UpdateUsdAssetLibrary(
		FString WhereId,
		FCMSUsdAssetLibraryUpdateInput UpdateData,
		const FOnGetUsdAssetLibrary& OnSuccess,
		const FOnFailureDelegate& OnFailure
	);
};
