#pragma once

#include "CoreMinimal.h"
#include "Repositories/BaseRepository.h"
#include "Structs/CMSTypes.h"

#include "SessionStateRepository.generated.h"

struct FCMSSessionStateWhereUniqueInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API USessionStateRepository : public UBaseRepository
{
	GENERATED_BODY()
	
public:
	
    void FindSession(
        const FCMSSessionStateWhereUniqueInput& Where,
        const TFunction<void(const TArray<FCMSSession>& Sessions)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    );
};
