
#pragma once

#include "CoreMinimal.h"

#include "CMSInputs.generated.h"

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString OwnerId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString About;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};


USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceUpdateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString About;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUsdScenePerformanceWhereInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString UsdSceneId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString PerformanceId;
};
