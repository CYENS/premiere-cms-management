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

extern const FString GQLSessionStateActiveId;
extern const FString GQLSessionStateInactiveId;

UENUM(BlueprintType)
enum class EGQLSessionState: uint8
{
	Inactive,
	Active,
};

FString GetSessionStateId(const EGQLSessionState& SessionState);
