#pragma once

#include "CoreMinimal.h"
#include "SessionRepository.h"
#include "Structs/CMSSession.h"
#include "PerformanceRepository.generated.h"

struct FCMSPerformanceCreateInput;
class UGraphQLDataSource;

DECLARE_DELEGATE_OneParam(FOnGetPerformanceSuccess, FCMSPerformance&);


UCLASS()
class PREMIERECMSMANAGEMENT_API UPerformanceRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "PremiereCMS")
    void Initialize(UGraphQLDataSource* InDataSource);
    
    void CreatePerformance(
        const FCMSPerformanceCreateInput& PerformanceCreateInput,
        FOnGetPerformanceSuccess OnSuccess,
        FOnFailure OnFailure
    );

private:
    UPROPERTY()
    UGraphQLDataSource* DataSource;
    
    static bool ParseCMSObjectFromResponse(const FString& JsonResponse, const FString& QueryName, FCMSPerformance& OutPerformance, FString& OutErrorReason);

    static bool CreatePerformanceFromSingleUserJsonObject(
        const TSharedPtr<FJsonObject>& SessionJsonObject,
        FCMSPerformance& OutPerformance,
        FString& OutErrorReason
    );
};
