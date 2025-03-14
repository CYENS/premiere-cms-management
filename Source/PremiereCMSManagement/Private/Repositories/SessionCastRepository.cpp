
#include "Repositories/SessionCastRepository.h"

#include "DataObjectBuilder.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSTypes.h"

void USessionCastRepository::Create(
	const FCMSAvatarWhereUniqueInput& AvatarWhere,
	const FCMSSessionWhereUniqueInput& SessionWhere,
	const FCMSUserWhereUniqueInput& UserWhere,
	const TFunction<void(const FCMSSessionCast& SessionCast)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("createSessionCast");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation CreateSessionCast ($data: SessionCastCreateInput!) {
	  %s (data: $data) {
		%s
      }
	}
	)"),
	*GQLSessionCastFragments,
	*QueryName,
	*GQLSessionCast
	);

	FDataObjectBuilder DataObjectBuilder = FDataObjectBuilder();
	DataObjectBuilder.AddConnect("avatar", AvatarWhere);
	DataObjectBuilder.AddConnect("session", SessionWhere);
	DataObjectBuilder.AddConnect("user", UserWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{
			"data", DataObjectBuilder.BuildAsJsonValue()
		}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}
