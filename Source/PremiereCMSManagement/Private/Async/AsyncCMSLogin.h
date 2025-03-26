#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "PremiereCMSManagementSubsystem.h"
#include "AsyncCMSLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginComplete, FString, ErrorMessage);

UCLASS()
class PREMIERECMSMANAGEMENT_API UAsyncCMSLogin: public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PremiereCMSManagement | Async", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly =  "true"))
	static UAsyncCMSLogin* AsyncLogin(UObject* WorldContextObject);
	
	UPROPERTY(BlueprintAssignable)
	FOnLoginComplete OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnLoginComplete OnFailure;
	
	virtual void Activate() override;

protected:
	UPROPERTY();
	UObject* WorldContextObject;


	UFUNCTION()
	void HandleSuccess();
	
	UFUNCTION()
	void HandleFailure(const FString& ErrorMessage);
};
