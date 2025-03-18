
#include "Repositories/AvatarRepository.h"

#include "DataObjectBuilder.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSTypes.h"
#include "Structs/CMSInputs.h"

void UAvatarRepository::Create(
	const FCMSAvatarCreateInput& AvatarCreateInput,
	const TOptional<FCMSPerformanceWhereUniqueInput>& PerformanceWhere,
	const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("createAvatar");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation CreateAvatar ($data: AvatarCreateInput!) {
	  %s (data: $data) {
		%s
      }
	}
	)"),
	*GQLAvatarFragments,
	*QueryName,
	*GQLAvatar
	);

	FDataObjectBuilder DataObjectBuilder = FDataObjectBuilder();
	DataObjectBuilder.AddUStruct(AvatarCreateInput);
	DataObjectBuilder.AddConnect("performances", PerformanceWhere);
	
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

void UAvatarRepository::GetAll(
	const TFunction<void(const TArray<FCMSAvatar>& Avatars)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure) const
{
	const FString QueryName = TEXT("avatars");
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query GetAll {
      %s {
		%s
      }
	}
	)"),
	*GQLSessionFragments,
	*QueryName,
	*GQLSession
	);
	ExecuteGraphQLQuery(
		Query,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UAvatarRepository::ConnectPerformance(
	const FCMSIdInput& AvatarWhere,
	const FCMSIdInput& PerformanceWhere,
	const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updateAvatar");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectPerformanceToAvatar ($where: AvatarWhereUniqueInput!, $data: AvatarUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLAvatarFragments,
	*QueryName,
	*GQLAvatar
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddConnect("performance", PerformanceWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(AvatarWhere)},
		{"data", DataObjectBuilder.BuildAsJsonValue()},
	};
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UAvatarRepository::DisconnectPerformance(
	const FCMSIdInput& AvatarWhere,
	const FCMSIdInput& PerformanceWhere,
	const TFunction<void(const FCMSAvatar& Avatar)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updateAvatar");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation DisconnectPerformanceFromAvatar ($where: AvatarWhereUniqueInput!, $data: AvatarUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLAvatarFragments,
	*QueryName,
	*GQLAvatar
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddDisconnect("performances");
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(AvatarWhere)},
		{"data", DataObjectBuilder.BuildAsJsonValue()},
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}
