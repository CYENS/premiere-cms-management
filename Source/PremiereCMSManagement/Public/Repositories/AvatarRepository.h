#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "AvatarRepository.generated.h"

struct FCMSIdInput;
struct FCMSAvatar;
struct FCMSAvatarCreateInput;
struct FCMSPerformanceWhereUniqueInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API UAvatarRepository : public UBaseRepository
{
    GENERATED_BODY()

public:

    void Create(
		const FCMSAvatarCreateInput& AvatarCreateInput,
		const TOptional<FCMSPerformanceWhereUniqueInput>& PerformanceWhere,
		const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
	
	void GetAll(
		const TFunction<void(const TArray<FCMSAvatar>& Avatars)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
	void ConnectPerformance(
		const FCMSIdInput& AvatarWhere,
		const FCMSIdInput& PerformanceWhere,
		const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
	void DisconnectPerformance(
		const FCMSIdInput& AvatarWhere,
		const FCMSIdInput& PerformanceWhere,
		const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;

};
