

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestCMSGameModeBase.generated.h"

class USessionRepository;
class UGraphQLDataSource;

UCLASS()
class PREMIERECMSMANAGEMENT_API ATestCMSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	void TestExecuteGraphQLQuery() const;
	void TestGetSessionById() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	UGraphQLDataSource* GraphQlDataSource;
	
	UPROPERTY()
	USessionRepository* SessionRepository;
	 
};
