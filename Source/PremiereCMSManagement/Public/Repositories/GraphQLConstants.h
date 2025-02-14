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
