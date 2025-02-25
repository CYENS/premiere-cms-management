#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "PersonRepository.generated.h"

struct FCMSPersonCreateInput;
struct FCMSPersonWhereUniqueInput;
struct FCMSUserWhereUniqueInput;
struct FCMSPerson;

UCLASS()
class PREMIERECMSMANAGEMENT_API UPersonRepository : public UBaseRepository
{
    GENERATED_BODY()

public:

    void Create(
		const FCMSPersonCreateInput& AvatarCreateInput,
		const TOptional<FCMSUserWhereUniqueInput>& UserWhere,
		const TFunction<void(const FCMSPerson& Avatar)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

	void Find(
		const FCMSPersonWhereUniqueInput& PersonWhere,
		const TFunction<void(const FCMSPerson& Avatar)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
	void WhereFamilyNameAndGivenName(
		const FString& FamilyName,
		const FString& GivenName,
		const TFunction<void(const TArray<FCMSPerson>& Avatar)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
	// void GetAll(
	// 	const TFunction<void(const TArray<FCMSAvatar>& Avatars)>& OnSuccess,
	// 	const TFunction<void(const FString& ErrorReason)>& OnFailure
	// ) const;
	
	// void ConnectPerformance(
	// 	const FCMSIdInput& AvatarWhere,
	// 	const FCMSIdInput& PerformanceWhere,
	// 	const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
	// 	const TFunction<void(const FString& ErrorReason)>& OnFailure
	// ) const;
	
	// void DisconnectPerformance(
	// 	const FCMSIdInput& AvatarWhere,
	// 	const FCMSIdInput& PerformanceWhere,
	// 	const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
	// 	const TFunction<void(const FString& ErrorReason)>& OnFailure
	// ) const;

};
