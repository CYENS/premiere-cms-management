#pragma once

#include "CoreMinimal.h"
#include "Structs/CMSTypes.h"
#include "PerformanceRepository.generated.h"

struct FCMSPerformanceCreateInput;
class UGraphQLDataSource;

DECLARE_DELEGATE_OneParam(FOnGetPerformanceSuccess, FCMSPerformance&);
DECLARE_DELEGATE_OneParam(FOnGetPerformancesSuccess, TArray<FCMSPerformance>&);
DECLARE_DELEGATE_OneParam(FOnFailure, FString);

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

    void GetAllPerformances(FOnGetPerformancesSuccess OnSuccess, FOnFailure OnFailure) const;

private:
    UPROPERTY()
    UGraphQLDataSource* DataSource;
    
    static bool ParseCMSObjectFromResponse(const FString& JsonResponse, const FString& QueryName, FCMSPerformance& OutPerformance, FString& OutErrorReason);

    static bool ParsePerformanceArrayFromResponse(
        const FString& ResponseContent,
        const FString& QueryName,
        TArray<FCMSPerformance>& OutPerformances,
        FString& OutErrorReason
    );

    static bool ParsePerformanceFromJsonObject(
        const TSharedPtr<FJsonObject>& JsonObject,
        FCMSPerformance& OutPerformance,
        FString& OutErrorReason
    );
    
};
