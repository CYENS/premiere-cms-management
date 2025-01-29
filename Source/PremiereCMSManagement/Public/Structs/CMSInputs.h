
#pragma once

#include "CoreMinimal.h"

#include "CMSInputs.generated.h"

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString OwnerId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString About;
};
