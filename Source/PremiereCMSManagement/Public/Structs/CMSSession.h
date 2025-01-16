
#pragma once

#include "CoreMinimal.h"

#include "CMSSession.generated.h"

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
};
