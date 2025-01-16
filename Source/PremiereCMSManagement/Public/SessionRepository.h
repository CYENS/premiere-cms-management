#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GraphQLDataSource.h"
#include "Structs/CMSSession.h"
#include "SessionRepository.generated.h"

DECLARE_DELEGATE_OneParam(FOnGetSessionSuccess, FCMSSession& /* Session */);
DECLARE_DELEGATE_OneParam(FOnFailure, FString /* ErrorReason */);

UCLASS()
class PREMIERECMSMANAGEMENT_API USessionRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Session Repository")
    void Initialize(UGraphQLDataSource* InDataSource);

    // UFUNCTION(BlueprintCallable, Category = "Session Repository")
    void GetSessionById(const FString& SessionId, FOnGetSessionSuccess OnSuccess, FOnFailure OnFailure) const;
    
    /**
     * Returns false if it fails to parse the response and OutErrorReason is populated
     */
    static bool ParseCMSSessionFromResponse(const FString& JsonResponse, FCMSSession& OutSession, FString& OutErrorReason);

private:
    UPROPERTY()
    UGraphQLDataSource* DataSource;

};
