
#pragma once

#include "CoreMinimal.h"

#include "CMSUser.generated.h"

struct FCMSPerformance;

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct  PREMIERECMSMANAGEMENT_API FCMSUser
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString EosId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Email;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString UserRole = "Read";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	bool IsSuperAdmin;
};
