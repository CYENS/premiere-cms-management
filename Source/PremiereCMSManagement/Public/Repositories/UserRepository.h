#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GraphQLDataSource.h"
#include "Structs/CMSSession.h"
#include "UserRepository.generated.h"

struct FCMSUser;

DECLARE_DELEGATE_OneParam(FOnGetUserSessionSuccess, FCMSUser& /* Session */);
// DECLARE_DELEGATE_OneParam(FOnGetActiveSessionsSuccess, TArray<FCMSUser> /* Sessions */);
DECLARE_DELEGATE_OneParam(FOnFailure, FString /* ErrorReason */);


UCLASS()
class PREMIERECMSMANAGEMENT_API UUserRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Session Repository")
    void Initialize(UGraphQLDataSource* InDataSource);
    
    void CreateUser(const FCMSUser& InUser, FOnGetUserSessionSuccess OnSuccess, FOnFailure OnFailure) const;

private:
    UPROPERTY()
    UGraphQLDataSource* DataSource;
    
    static bool ParseCMSUserFromResponse(const FString& JsonResponse, const FString& QueryName, FCMSUser& OutUser, FString& OutErrorReason);

    static bool CreateSessionFromSingleUserJsonObject(
        const TSharedPtr<FJsonObject>& SessionJsonObject,
        FCMSUser& OutUser,
        FString& OutErrorReason
    );
};
