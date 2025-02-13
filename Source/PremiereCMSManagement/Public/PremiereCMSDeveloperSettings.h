#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PremiereCMSDeveloperSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Premiere CMS Managmement"))
class PREMIERECMSMANAGEMENT_API UPremiereCMSDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// This is the category that shows up in Project Settings
	// e.g., "Project -> Plugins -> Premiere CMS Management"
	// or "Plugins -> My Plugin" depending on Editor version
	UPremiereCMSDeveloperSettings();

	UPROPERTY(EditAnywhere, Config, Category = "General")
	FString GraphQLUrl;
	
};
