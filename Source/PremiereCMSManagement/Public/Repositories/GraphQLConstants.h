// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

extern const FString GQLUserFragments;
extern const FString GQLUser;
extern const FString GQLPerformanceFragments;
extern const FString GQLPerformance;
extern const FString GQLSessionFragments;
extern const FString GQLSession;
extern const FString GQLUsdSceneFragments;
extern const FString GQLUsdScene;
extern const FString GQLAvatarFragments;
extern const FString GQLAvatar;
extern const FString GQLPersonFragments;
extern const FString GQLPerson;
extern const FString GQLSessionCastFragments;
extern const FString GQLSessionCast;
extern const FString GQLUsdAssetLibraryFragments;
extern const FString GQLUsdAssetLibrary;
extern const TMap<FString, FString> GQLSelectionSets;

extern const FString GQLSessionStatePublicActiveId;
extern const FString GQLSessionStatePrivateInactiveId;

UENUM(BlueprintType)
enum class EGQLSessionState: uint8
{
	PrivateInactive,
	PublicActive,
	PrivateActive,
	PublicInactive,
};

FString GetSessionStateId(const EGQLSessionState& SessionState);
EGQLSessionState GetSessionStateFromSessionStateId(const FString& SessionStateId);

FString GetObjectSelectionSetFromType(const FString& ObjectType);
FString GetObjectFragmentsFromType(const FString& ObjectType);
