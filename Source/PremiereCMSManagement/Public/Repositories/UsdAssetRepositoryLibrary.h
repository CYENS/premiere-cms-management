#pragma once

#include "CoreMinimal.h"
#include "Repositories/BaseRepository.h"
#include "UObject/Object.h"
#include "UsdAssetRepositoryLibrary.generated.h"

UCLASS()
class PREMIERECMSMANAGEMENT_API UUsdAssetLibraryRepository : public UBaseRepository
{
	GENERATED_BODY()
protected:
	virtual FString GetObjectType() const override;
};
