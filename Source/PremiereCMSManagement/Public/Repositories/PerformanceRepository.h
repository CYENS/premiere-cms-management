#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "Structs/CMSTypes.h"
#include "PerformanceRepository.generated.h"

struct FCMSUsdSceneWhereUniqueInput;
struct FCMSUserWhereUniqueInput;
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
    
    void ConnectUsdScenes(
        const FCMSIdInput& PerformanceWhereUniqueInput,
        const TArray<FCMSIdInput>& UsdScenesToConnectUsdSceneWhereUniqueInputs,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void ConnectSessions(
        const FCMSIdInput& PerformanceWhereUniqueInput,
        const TArray<FCMSIdInput>& SessionsScenesToConnectUsdSceneWhereUniqueInputs,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void DisconnectSessions(const FCMSIdInput& PerformanceWhereUniqueInput,
        const TArray<FCMSIdInput>& SessionsWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void ConnectUsdScenes(
        const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
        const TArray<FCMSUsdSceneWhereUniqueInput>& UsdScenesWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void DisconnectUsdScenes(
        const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	    const TArray<FCMSUsdSceneWhereUniqueInput>& UsdScenesWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void ConnectMembers(
	    const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	    const TArray<FCMSUserWhereUniqueInput>& MembersWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void DisconnectMembers(
        const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	    const TArray<FCMSUserWhereUniqueInput>& MembersWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void ConnectAvatars(
        const FCMSIdInput& PerformanceWhere,
        const TArray<FCMSIdInput>& AvatarsWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    void DisconnectAvatars(
        const FCMSIdInput& PerformanceWhere,
        const TArray<FCMSIdInput>& AvatarsWhere,
        const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
};

