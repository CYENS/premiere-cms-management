#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "UObject/NoExportTypes.h"

#include "SessionRepository.generated.h"

enum class EGQLSessionState: uint8;
struct FCMSSession;
struct FCMSSessionCreateInput;
struct FCMSSessionUpdateInput;
struct FCMSSessionWhereUniqueInput;
struct FCMSUsdSceneWhereUniqueInput;
struct FCMSUserWhereUniqueInput;
struct FCMSIdInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API USessionRepository : public UBaseRepository
{
    GENERATED_BODY()

public:
    void GetAll(
        const TFunction<void(const TArray<FCMSSession>& Sessions)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    void FindSession(
        const FCMSSessionWhereUniqueInput& Where,
	    const TFunction<void(const FCMSSession& Session)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
	void FindByEosSessionId(
		const FString& WhereEosSessionId,
		const TFunction<void(const TArray<FCMSSession>& Sessions)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
    
    void GetActiveSessions(
        const TFunction<void(const TArray<FCMSSession>& Sessions)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void CreateSession(
		const FCMSSessionCreateInput& Data,
		const FCMSIdInput& SessionStateWhereId,
		const TOptional<FCMSIdInput>& OwnerWhere,
		const TOptional<FCMSIdInput>& UsdSceneWhere,
		const TArray<FCMSIdInput>& AudioDataWhereIds,
		const TArray<FCMSIdInput>& FaceDataWhereIds,
        const TFunction<void(const FCMSSession& Sessions)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    void UpdateSession(
		const FCMSSessionWhereUniqueInput& SessionWhere,
		const FCMSSessionUpdateInput& Data,
		const FCMSIdInput& SessionStateWhereId,
		const TOptional<FCMSUserWhereUniqueInput>& OwnerWhere,
		const TOptional<FCMSUsdSceneWhereUniqueInput>& UsdSceneWhere,
		const TArray<FCMSIdInput>& AudioDataWhereIds,
		const TArray<FCMSIdInput>& FaceDataWhereIds,
	    const TFunction<void(const FCMSSession& Session)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
	
    void UpdateSessionStateByEosSessionId(
		const FString& WhereEosSessionId,
		const EGQLSessionState& SessionState,
	    const TFunction<void(const FCMSSession& Session)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
};
