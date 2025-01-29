
#pragma once

#include "CoreMinimal.h"

#include "CMSTypes.generated.h"

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSDateTime
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Value;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSPerson
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString GivenName;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FamilyName;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString ArtisticName;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUserAttendance
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString UserId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString SessionId;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarMotionData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString SessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FVector InitialPosition;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FRotator InitialRotation;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString AvatarId;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSLightData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString SessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    int32 LightId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FVector InitialPosition;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FRotator InitialRotation;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString LightType;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString LightCharacteristicsJson;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSFaceData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString SessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString AvatarId;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAudioData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString SessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString AvatarId;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSPropMotionData
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString SessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FVector InitialPosition;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FRotator InitialRotation;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUSDScene
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString OwnerId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FString> MemberIds;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    bool Template;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    bool Public;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FString> PerformanceIds;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUSDAssetLibrary
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString AssetLibraryJson;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSXRLive
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString StreamingUrl;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAvatar
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Name;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSPerformance
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString About;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString OwnerId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FString> MemberIds;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FString> UsdSceneIds;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FString> SessionIds;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FString> AvatarIds;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionState
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Name;
};
