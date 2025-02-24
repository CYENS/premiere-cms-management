#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PremiereCMSManagementSubsystem.h"
#include "AsyncGetAllSessions.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompleteRequest, const TArray<FCMSSession>&, Sessions, FString, ErrorMessage);

UCLASS()
class PREMIERECMSMANAGEMENT_API UAsyncGetAllSessions : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PremiereCMSManagement | Async | Session", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly =  "true"))
	static UAsyncGetAllSessions* AsyncGetAllSessions(UObject* WorldContextObject);
	
	UPROPERTY(BlueprintAssignable)
	FOnCompleteRequest OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnCompleteRequest OnFailure;
	
	virtual void Activate() override;

protected:
	UPROPERTY();
	UObject* WorldContextObject;


	UFUNCTION()
	void HandleSuccess(const TArray<FCMSSession>& Sessions);
	
	UFUNCTION()
	void HandleFailure(const FString& ErrorMessage);
};
