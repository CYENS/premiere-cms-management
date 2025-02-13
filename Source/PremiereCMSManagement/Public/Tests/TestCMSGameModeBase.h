

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "PremiereCMSDeveloperSettings.h"
#include "GameFramework/GameModeBase.h"
#include "Repositories/PerformanceRepository.h"
#include "Repositories/UsdSceneRepository.h"
#include "Repositories/UserRepository.h"
#include "TestCMSGameModeBase.generated.h"

struct FCMSSession;
class USessionRepository;
class UGraphQLDataSource;

UCLASS()
class PREMIERECMSMANAGEMENT_API ATestCMSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TestGetActiveSessions();
	
	UFUNCTION(BlueprintCallable)
	void TestExecuteGraphQLQuery();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UPremiereCMSDeveloperSettings* DeveloperSettings;
	
	UPROPERTY()
	FString GraphQLUrl;
	
	UPROPERTY()
	UGraphQLDataSource* GraphQlDataSource;
	
	UPROPERTY()
	USessionRepository* SessionRepository;
	
	UPROPERTY()
	UUserRepository* UserRepository;
	
	UPROPERTY()
	UPerformanceRepository* PerformanceRepository;
	
	UPROPERTY()
	UUsdSceneRepository* UsdSceneRepository;
	
	template<typename U>
	static void LogUStructs(const TArray<U>& Structs);
	
	template<typename U>
	static void LogUStruct(const U& Struct);
	
	static void LogError(const FString& ErrorReason);

};

