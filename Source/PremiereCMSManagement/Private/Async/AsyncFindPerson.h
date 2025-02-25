#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PremiereCMSManagementSubsystem.h"
#include "AsyncFindPerson.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestComplete, const FCMSPerson&, Person, FString, ErrorMessage);

UCLASS()
class PREMIERECMSMANAGEMENT_API UAsyncFindPerson: public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	FCMSPersonWhereUniqueInput Where;
	
public:
	UFUNCTION(BlueprintCallable, Category = "PremiereCMSManagement | Async | Session", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly =  "true"))
	static UAsyncFindPerson* AsyncFindPerson(UObject* WorldContextObject, const FCMSPersonWhereUniqueInput& Where);
	
	UPROPERTY(BlueprintAssignable)
	FOnRequestComplete OnFindSuccess;
	
	UPROPERTY(BlueprintAssignable)
	FOnRequestComplete OnNotFound;

	UPROPERTY(BlueprintAssignable)
	FOnRequestComplete OnFailure;
	
	virtual void Activate() override;

protected:
	UPROPERTY();
	UObject* WorldContextObject;

	UFUNCTION()
	void HandleSuccess(const FCMSPerson& Person);
	
	UFUNCTION()
	void HandleFailure(const FString& ErrorMessage);
};
