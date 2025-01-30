#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GraphQLDataSource.h"
#include "Structs/CMSSession.h"
#include "SessionRepository.generated.h"

DECLARE_DELEGATE_OneParam(FOnGetSessionSuccess, FCMSSession& /* Session */);
DECLARE_DELEGATE_OneParam(FOnGetActiveSessionsSuccess, TArray<FCMSSession> /* Sessions */);
DECLARE_DELEGATE_OneParam(FOnFailure, FString /* ErrorReason */);

UCLASS()
class PREMIERECMSMANAGEMENT_API USessionRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Session Repository")
    void Initialize(UGraphQLDataSource* InDataSource);

    void GetSessionById(const FString& SessionId, FOnGetSessionSuccess OnSuccess, FOnFailure OnFailure) const;

    void GetActiveSessions(FOnGetActiveSessionsSuccess OnSuccess, FOnFailure OnFailure) const;

    void CreateSession(
        const FCMSSession& InSession,
        FOnGetSessionSuccess OnSuccess,
        FOnFailure OnFailure
    ) const;

private:
    UPROPERTY()
    UGraphQLDataSource* DataSource;
    
    /**
     * Returns false if it fails to parse the response and OutErrorReason is populated
     */
    static bool ParseCMSSessionFromResponse(const FString& JsonResponse, const FString& QueryName, FCMSSession& OutSession, FString& OutErrorReason);

    static bool ParseCMSMultipleSessionsFromResponse(
        const FString& JsonResponse,
        const FString& QueryName,
        TArray<FCMSSession>& OutSessions,
        FString& OutErrorReason
    );

    static bool CreateSessionFromSingleSessionJsonObject(
        const TSharedPtr<FJsonObject>& SessionJsonObject,
        FCMSSession& OutSession,
        FString& OutErrorReason
    );
    
    static bool TryExtractIdsFromSessionObject(
        const TSharedPtr<FJsonObject>& SessionJsonObject,
        const FString& FieldName,
        TArray<FString>& Ids
    );
};
