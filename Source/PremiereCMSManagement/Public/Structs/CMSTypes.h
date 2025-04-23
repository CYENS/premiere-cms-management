
#pragma once

#include "CoreMinimal.h"
#include "CMSTypes.generated.h"

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionState
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Name;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUserForPerson
{
    GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString EosId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString Email;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString UserRole = "Read";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString AdminPermissions;
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
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FCMSUserForPerson User;
};


USTRUCT(BlueprintType, Category = "PremiereCMS")
struct  PREMIERECMSMANAGEMENT_API FCMSUser
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString EosId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString Email;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString Name;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FString UserRole = "Read";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	bool IsAdmin;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	bool IsSuperAdmin;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS")
	FCMSPerson Person; 
};


USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSDateTime
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Value;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarForAvatarMotionData
{
    GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString AssetId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Name;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAvatarMotionData
{
    GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString FileUrl;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	float InitialPositionX;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	float InitialPositionY;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	float InitialPositionZ;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	float InitialRotationX;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	float InitialRotationY;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	float InitialRotationZ;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FCMSAvatarForAvatarMotionData Avatar;
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
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceForSession
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString About;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser Owner;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionCastForAvatar
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser User;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser Session;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAvatar
{
    GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString AssetId;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Name;

    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FCMSSessionCastForAvatar> SessionCast;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionCastForSession
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSAvatar Avatar;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser User;
};


USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionForUsdScene
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString EosSessionId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString StreamingUrl;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	TArray<FString> AudioDataIds;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSPerformanceForSession Performance;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSUser Owner;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSSessionState State;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	TArray<FCMSSessionCastForSession> CastAvatars;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUsdScene
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
    bool Template;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    bool Public;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser Owner;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FCMSSessionForUsdScene> Scene;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FCMSUser> Members;
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
struct PREMIERECMSMANAGEMENT_API FCMSPerformanceForAvatar
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Title;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString About;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser Owner;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionCast
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSAvatar Avatar;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser User;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser Session;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUsdSceneForSession
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
    bool Template;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    bool Public;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSUser Owner;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FCMSUser> Members;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSession
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString EosSessionId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString StreamingUrl;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Backdrop;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	TArray<FString> AudioDataIds;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSPerformanceForSession Performance;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSUser Owner;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSUsdSceneForSession Scene;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSSessionState State;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	TArray<FCMSSessionCastForSession> CastAvatars;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSSessionForPerformance
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString EosSessionId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString StreamingUrl;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FString Backdrop;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	TArray<FString> AudioDataIds;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSUser Owner;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	FCMSSessionState State;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PremiereCMS | Types")
	TArray<FCMSSessionCastForSession> CastAvatars;
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
	FCMSUser Owner;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FCMSUser> Members;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FCMSUsdScene> UsdScenes;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FCMSSessionForPerformance> Sessions;
    
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    TArray<FCMSAvatar> Avatars;
};


USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSFile
{
	GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Filename;

	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Filesize;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSObjectWithFile
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString Id;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString FileUrl;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString PCloudFileId;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FString UploadAt;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSFile File;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSAssetLibraryJson
{
	GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FString> Objects;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FString> Furniture;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	TArray<FString> Nature;
};

USTRUCT(BlueprintType, Category = "PremiereCMS | Types")
struct PREMIERECMSMANAGEMENT_API FCMSUsdAssetLibrary
{
    GENERATED_BODY()
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString Id;
	
    UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
    FString PCloudFileId;
	
	UPROPERTY(BlueprintReadWrite, Category = "PremiereCMS | Types")
	FCMSAssetLibraryJson AssetLibraryJson;
};
