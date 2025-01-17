

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestCMSGameModeBase.generated.h"

struct FCMSSession;
class USessionRepository;
class UGraphQLDataSource;

UCLASS()
class PREMIERECMSMANAGEMENT_API ATestCMSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	void TestExecuteGraphQLQuery() const;
	void TestGetSessionById() const;
	void TestGetActiveSessions() const;
	
public:
	// UFUNCTION(BlueprintCallable, Category = "TestCMSGameModeBase")
	// void CreateSession(
	// 	const FString& Title,
	// 	const FString& OwnerId,
	// 	const FString& PerformanceId
	// ) const;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	UGraphQLDataSource* GraphQlDataSource;
	
	UPROPERTY()
	USessionRepository* SessionRepository;

	static void LogSession(const FCMSSession& Session);
	 
};
