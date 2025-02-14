#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "Structs/CMSTypes.h"
#include "PerformanceRepository.generated.h"

struct FCMSIdInput;
struct FCMSPerformanceUpdateInput;
struct FCMSPerformanceCreateInput;
struct FCMSPerformanceWhereUniqueInput;
struct FCMSUsdScenePerformanceWhereInput;
struct FCMSUserPerformanceWhereUniqueInput;

class UGraphQLDataSource;

UCLASS()
class PREMIERECMSMANAGEMENT_API UPerformanceRepository : public UBaseRepository
{
    GENERATED_BODY()

public:
    void CreatePerformance(
        const FCMSPerformanceCreateInput& PerformanceCreateInput,
        const FCMSIdInput& OwnerWhereUniqueInput,
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
    
    void AddUsdScene(
        const FCMSUsdScenePerformanceWhereInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void RemoveUsdScene(
        const FCMSUsdScenePerformanceWhereInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void AddMember(
	    const FCMSUserPerformanceWhereUniqueInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void RemoveMember(
	    const FCMSUserPerformanceWhereUniqueInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

};

