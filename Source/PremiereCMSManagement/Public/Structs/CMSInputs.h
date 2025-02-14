
#pragma once

#include "CoreMinimal.h"

#include "CMSInputs.generated.h"

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString About;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUsdSceneCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString OwnerId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    bool Public;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    bool Template;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUsdSceneUpdateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString OwnerId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    bool Public;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    bool Template;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};


USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUsdSceneWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString PCloudFileId;
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

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUserPerformanceWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString UserId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString PerformanceId;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs | Session")
struct PREMIERECMSMANAGEMENT_API FCMSSessionWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs | Session")
struct PREMIERECMSMANAGEMENT_API FCMSSessionCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString EosSessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString PerformanceStreamingUrl;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSIdInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};


USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct FCMSSessionStateWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};
