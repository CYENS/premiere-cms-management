
#include "Repositories/PersonRepository.h"

#include "DataObjectBuilder.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSTypes.h"
#include "Structs/CMSInputs.h"

void UPersonRepository::Create(
	const FCMSPersonCreateInput& AvatarCreateInput,
	const TOptional<FCMSUserWhereUniqueInput>& UserWhere,
	const TFunction<void(const FCMSPerson& Person)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("createPerson");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation CreatePerson ($data: PersonCreateInput!) {
	  %s (data: $data) {
		%s
      }
	}
	)"),
	*GQLPersonFragments,
	*QueryName,
	*GQLPerson
	);

	FDataObjectBuilder DataObjectBuilder = FDataObjectBuilder();
	DataObjectBuilder.AddUStruct(AvatarCreateInput);
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

// void UAvatarRepository::GetAll(
// 	const TFunction<void(const TArray<FCMSAvatar>& Avatars)>& OnSuccess,
// 	const TFunction<void(const FString& ErrorReason)>& OnFailure) const
// {
// 	const FString QueryName = TEXT("avatars");
// 	const FString Query = FString::Printf(TEXT(R"(
// 	%s
//     query GetAll {
//       %s {
// 		%s
//       }
// 	}
// 	)"),
// 	*GQLSessionFragments,
// 	*QueryName,
// 	*GQLSession
// 	);
// 	ExecuteGraphQLQuery(
// 		Query,
// 		QueryName,
// 		OnSuccess,
// 		OnFailure
// 	);
// }
//
// void UAvatarRepository::ConnectPerformance(
// 	const FCMSIdInput& AvatarWhere,
// 	const FCMSIdInput& PerformanceWhere,
// 	const TFunction<void(const FCMSAvatar& Person)>& OnSuccess,
// 	const TFunction<void(const FString& ErrorReason)>& OnFailure
// ) const
// {
// 	const FString QueryName = TEXT("updateAvatar");
// 	const FString Query = FString::Printf(TEXT(R"(
// 	%s
// 	mutation ConnectPerformanceToAvatar ($where: AvatarWhereUniqueInput!, $data: AvatarUpdateInput!) {
// 	  %s (where: $where, data: $data) {
// 		%s
// 	  }
// 	}
// 	)"),
// 	*GQLAvatarFragments,
// 	*QueryName,
// 	*GQLAvatar
// 	);
//
// 	FDataObjectBuilder DataObjectBuilder;
// 	DataObjectBuilder.AddConnect("performance", PerformanceWhere);
// 	
// 	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
// 		{"where", MakeWhereValue(AvatarWhere)},
// 		{"data", DataObjectBuilder.BuildAsJsonValue()},
// 	};
// 	ExecuteGraphQLQuery(
// 		Query,
// 		Variables,
// 		QueryName,
// 		OnSuccess,
// 		OnFailure
// 	);
// }
//
// void UAvatarRepository::DisconnectPerformance(
// 	const FCMSIdInput& AvatarWhere,
// 	const FCMSIdInput& PerformanceWhere,
// 	const TFunction<void(const FCMSAvatar& Person)>& OnSuccess,
// 	const TFunction<void(const FString& ErrorReason)>& OnFailure
// ) const
// {
// 	const FString QueryName = TEXT("updateAvatar");
// 	const FString Query = FString::Printf(TEXT(R"(
// 	%s
// 	mutation DisconnectPerformanceFromAvatar ($where: AvatarWhereUniqueInput!, $data: AvatarUpdateInput!) {
// 	  %s (where: $where, data: $data) {
// 		%s
// 	  }
// 	}
// 	)"),
// 	*GQLAvatarFragments,
// 	*QueryName,
// 	*GQLAvatar
// 	);
//
// 	FDataObjectBuilder DataObjectBuilder;
// 	DataObjectBuilder.AddDisconnect("performance", PerformanceWhere);
// 	
// 	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
// 		{"where", MakeWhereValue(AvatarWhere)},
// 		{"data", DataObjectBuilder.BuildAsJsonValue()},
// 	};
// 	
// 	ExecuteGraphQLQuery(
// 		Query,
// 		Variables,
// 		QueryName,
// 		OnSuccess,
// 		OnFailure
// 	);
// }
