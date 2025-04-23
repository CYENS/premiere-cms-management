#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "UObject/NoExportTypes.h"

#include "AvatarMotionDataRepository.generated.h"

UCLASS()
class PREMIERECMSMANAGEMENT_API UAvatarMotionDataRepository : public UBaseRepository
{
    GENERATED_BODY()

protected:
	virtual FString GetObjectType() const override;
};
