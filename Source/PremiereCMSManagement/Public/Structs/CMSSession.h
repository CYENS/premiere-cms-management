
#pragma once

#include "CoreMinimal.h"

#include "CMSSession.generated.h"

struct FCMSPerformance;

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct  PREMIERECMSMANAGEMENT_API FCMSSession
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "PremiereCMS | Types | CMSSession")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString EosSessionId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString State = "inactive";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString StreamingUrl;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	TArray<FString> AudioDataIds;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString PerformanceId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString OwnerId;

};
