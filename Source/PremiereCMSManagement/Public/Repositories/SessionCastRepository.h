#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "SessionCastRepository.generated.h"

struct FCMSSessionCast;
struct FCMSAvatarWhereUniqueInput;
struct FCMSUserWhereUniqueInput;
struct FCMSSessionWhereUniqueInput;

UCLASS()
class PREMIERECMSMANAGEMENT_API USessionCastRepository : public UBaseRepository
{
    GENERATED_BODY()

public:

    void Create(
		const FCMSAvatarWhereUniqueInput& AvatarWhere,
		const FCMSSessionWhereUniqueInput& SessionWhere,
		const FCMSUserWhereUniqueInput& UserWhere,
		const TFunction<void(const FCMSSessionCast& SessionCast)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
};
