#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "UObject/NoExportTypes.h"

#include "AudioDataRepository.generated.h"

UCLASS()
class PREMIERECMSMANAGEMENT_API UAudioDataRepository : public UBaseRepository
{
    GENERATED_BODY()

protected:
	virtual FString GetObjectType() const override;
};
