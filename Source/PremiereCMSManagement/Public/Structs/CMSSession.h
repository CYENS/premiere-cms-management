
#pragma once

#include "CoreMinimal.h"

#include "CMSSession.generated.h"

struct FCMSPerformance;

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct  PREMIERECMSMANAGEMENT_API FCMSSession
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString StreamingUrl;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString AudioDataId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString AudioDataFileUrl;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString PerformanceId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString PerformanceTitle;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types | CMSSession")
	FString PerformanceDescription;
};
