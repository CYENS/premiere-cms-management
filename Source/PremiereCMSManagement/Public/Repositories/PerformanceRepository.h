#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "Structs/CMSTypes.h"
#include "PerformanceRepository.generated.h"

struct FCMSPerformanceUpdateInput;
struct FCMSPerformanceCreateInput;
struct FCMSPerformanceWhereUniqueInput;

class UGraphQLDataSource;

DECLARE_DELEGATE_OneParam(FOnGetPerformanceSuccess, const FCMSPerformance&);
DECLARE_DELEGATE_OneParam(FOnGetPerformancesSuccess, TArray<FCMSPerformance>&);
DECLARE_DELEGATE_OneParam(FOnFailure, FString);

UCLASS()
class PREMIERECMSMANAGEMENT_API UPerformanceRepository : public UBaseRepository
{
    GENERATED_BODY()

public:
    void CreatePerformance(
        const FCMSPerformanceCreateInput& PerformanceCreateInput,
	    const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    void GetAllPerformances(
	    const TFunction<void(const TArray<FCMSPerformance>& Performances)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void FindPerformance(
        const FCMSPerformanceWhereUniqueInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void DeletePerformance(
        const FCMSPerformanceWhereUniqueInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void UpdatePerformance(
        const FCMSPerformanceWhereUniqueInput& Where,
	    const FCMSPerformanceUpdateInput& Data,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

};

