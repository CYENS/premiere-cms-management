// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Repositories/GraphQLConstants.h"
#include "PremiereCMSManagementBlueprintLibrary.generated.h"

struct FCMSSessionState;

UCLASS()
class PREMIERECMSMANAGEMENT_API UPremiereCMSManagementBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="PremiereCMSManagement")
	static EGQLSessionState GetSessionStateFromId(const FString& SessionStateId);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="PremiereCMSManagement")
	static EGQLSessionState GetSessionStateFromStruct(const FCMSSessionState& SessionState);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="PremiereCMSManagement")
	static FString GetSessionStateIdFromSessionState(EGQLSessionState SessionState);
};
