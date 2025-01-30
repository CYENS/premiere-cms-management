#pragma once

#include "CoreMinimal.h"
#include "GraphQLDataSource.h"
#include "UserRepository.generated.h"

struct FCMSUser;

DECLARE_DELEGATE_OneParam(FOnGetUserSuccess, FCMSUser&);
DECLARE_DELEGATE_OneParam(FOnGetUsersSuccess, TArray<FCMSUser>&);
DECLARE_DELEGATE_OneParam(FOnFailure, FString);

UCLASS()
class PREMIERECMSMANAGEMENT_API UUserRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Session Repository")
    void Initialize(UGraphQLDataSource* InDataSource);
    
    void GetAllUsers(FOnGetUsersSuccess OnSuccess, FOnFailure OnFailure) const;
    
    void CreateUser(const FCMSUser& InUser, FOnGetUserSuccess OnSuccess, FOnFailure OnFailure) const;

private:
    UPROPERTY()
    UGraphQLDataSource* DataSource;
    
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
