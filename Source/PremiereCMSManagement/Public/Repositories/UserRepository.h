#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "GraphQLConstants.h"
#include "GraphQLDataSource.h"
#include "UserRepository.generated.h"

struct FCMSIdInput;
struct FCMSUser;

DECLARE_DELEGATE_OneParam(FOnGetUserSuccess, FCMSUser&);
DECLARE_DELEGATE_OneParam(FOnGetUsersSuccess, TArray<FCMSUser>&);
DECLARE_DELEGATE_OneParam(FOnFailure, FString);

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
	
    void CreateUser(const FCMSUser& InUser, FOnGetUserSuccess OnSuccess, FOnFailure OnFailure) const;

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
