#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "UObject/NoExportTypes.h"

#include "SessionRepository.generated.h"

struct FCMSSession;
struct FCMSSessionCreateInput;
struct FCMSSessionWhereUniqueInput;
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
	    const FCMSIdInput& OwnerWhereId,
	    const TArray<FCMSIdInput>& AudioDataWhereIds,
        const TArray<FCMSIdInput>& FaceDataWhereIds,
        const TFunction<void(const FCMSSession& Sessions)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
};
