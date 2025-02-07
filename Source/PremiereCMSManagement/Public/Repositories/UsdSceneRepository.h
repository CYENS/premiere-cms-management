#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "Structs/CMSTypes.h"

#include "UsdSceneRepository.generated.h"

struct FCMSUsdSceneCreateInput;
struct FCMSPerformanceUpdateInput;
struct FCMSPerformanceWhereUniqueInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API UUsdSceneRepository : public UBaseRepository
{
    GENERATED_BODY()

public:
    void Create(
	    const FCMSUsdSceneCreateInput& UsdSceneCreateInput,
	    const TFunction<void(const FCMSUsdScene& UsdScene)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    void GetAll(
	    const TFunction<void(const TArray<FCMSUsdScene>& UsdScenes)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void Find(
        const FCMSPerformanceWhereUniqueInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void Delete(
        const FCMSPerformanceWhereUniqueInput& Where,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void Update(
        const FCMSPerformanceWhereUniqueInput& Where,
	    const FCMSPerformanceUpdateInput& Data,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

};

