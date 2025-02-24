#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "UserRepository.generated.h"

struct FCMSUserWhereUniqueInput;
struct FCMSPerformanceUpdateInput;
struct FCMSIdInput;
struct FCMSUser;
struct FCMSUserUpdateInput;
struct FCMSUserCreateInput;
struct FCMSPersonWhereUniqueInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API UUserRepository : public UBaseRepository
{
    GENERATED_BODY()

public:
    
	void GetAll(
		const TFunction<void(const TArray<FCMSUser>& Users)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
	void Find(
		const FCMSIdInput& Where,
		const TFunction<void(const FCMSUser& User)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
    void Create(
		const FCMSUserCreateInput& Data,
		const TOptional<FCMSIdInput>& PersonWhereId,
		const TFunction<void(const FCMSUser& User)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
	
	void Update(
		const FCMSIdInput& Where,
		const FCMSPerformanceUpdateInput& Data,
		const TFunction<void(const FCMSUser& User)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;

	void ConnectPerson(
		const FCMSUserWhereUniqueInput& UserWhere,
		const FCMSPersonWhereUniqueInput& PersonWhere,
		const TFunction<void(const FCMSUser& User)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	);
	
	void DisconnectPerson(
		const FCMSUserWhereUniqueInput& UserWhere,
		const FCMSPersonWhereUniqueInput& PersonWhere,
		const TFunction<void(const FCMSUser& User)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	);

private:
    static bool ParseCMSUserFromResponse(const FString& JsonResponse, const FString& QueryName, FCMSUser& OutUser, FString& OutErrorReason);
    static bool ParseMultipleCMSUsersFromResponse(
        const FString& JsonResponse,
        const FString& QueryName,
        TArray<FCMSUser>& OutUsers,
        FString& OutErrorReason
    );

    static bool ParseCMSUserFromSingleUserJsonObject(
        const TSharedPtr<FJsonObject>& SessionJsonObject,
        FCMSUser& OutUser,
        FString& OutErrorReason
    );
};
