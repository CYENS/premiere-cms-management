
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

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUserWhereUniqueInput
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs | Session")
struct PREMIERECMSMANAGEMENT_API FCMSSessionWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSSessionCastWhereUniqueInput
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
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString Backdrop;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs | Session")
struct PREMIERECMSMANAGEMENT_API FCMSSessionUpdateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString EosSessionId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString PerformanceStreamingUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs | Session")
    FString Title;
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

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUserUpdateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString UserRole;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Email;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString EosId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString AdminPermissions;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString ActionPermissions;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUserCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Name;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Email;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString EosId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Password;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString UserRole = TEXT("Viewer");
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString AdminPermissions;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString ActionPermissions = TEXT("read");
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarMotionDataCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Label;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    float InitialPositionX;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    float InitialPositionY;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    float InitialPositionZ;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    float InitialRotationX;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    float InitialRotationY;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    float InitialRotationZ;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSAudioDataCreateInput
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString AssetId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Name;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarUpdateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Name;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPersonCreateInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString ArtisticName;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Bio;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Gender;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString FamilyName;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString GivenName;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSPersonWhereUniqueInput
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Inputs")
    FString Id;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAssetLibraryJsonCreateInput
{
	GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FString> Objects;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FString> Furniture;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FString> Nature;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUsdAssetLibraryCreateInput
{
    GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSAssetLibraryJsonCreateInput AssetLibraryJson;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Inputs")
struct PREMIERECMSMANAGEMENT_API FCMSUsdAssetLibraryUpdateInput
{
    GENERATED_BODY()
    
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSAssetLibraryJsonCreateInput AssetLibraryJson;
};
