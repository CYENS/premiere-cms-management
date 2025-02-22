#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "Structs/CMSTypes.h"

#include "UsdSceneRepository.generated.h"

struct FCMSIdInput;
struct FCMSUsdSceneCreateInput;
struct FCMSPerformanceUpdateInput;
struct FCMSPerformanceWhereUniqueInput;
struct FCMSUsdSceneUpdateInput;
struct FCMSUsdSceneWhereUniqueInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API UUsdSceneRepository : public UBaseRepository
{
    GENERATED_BODY()

public:
    void Create(
	    const FCMSUsdSceneCreateInput& UsdSceneCreateInput,
		const TOptional<FCMSIdInput>& OwnerUniqueWhereInput,
	    const TFunction<void(const FCMSUsdScene& UsdScene)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    void GetAll(
	    const TFunction<void(const TArray<FCMSUsdScene>& UsdScenes)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void Find(
		const FCMSUsdSceneWhereUniqueInput& Where,
		const TFunction<void(const FCMSUsdScene& UsdScene)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void Delete(
		const FCMSUsdSceneWhereUniqueInput& Where,
		const TFunction<void(const FCMSUsdScene& UsdScene)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void Update(
		const FCMSUsdSceneWhereUniqueInput& Where,
		const FCMSUsdSceneUpdateInput& Data,
		const TFunction<void(const FCMSUsdScene& Performance)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
};

