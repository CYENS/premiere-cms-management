
#include "Repositories/PerformanceRepository.h"

#include "DataObjectBuilder.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

void UPerformanceRepository::CreatePerformance(
	const FCMSPerformanceCreateInput& PerformanceCreateInput,
	const FCMSIdInput& OwnerWhereUniqueInput,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation CreatePerformanceMutation($data: PerformanceCreateInput!) {
	  createPerformance(data: $data) {
		%s
      }
	}
	)"),
	*GQLPerformanceFragments,
	*GQLPerformance
	);
	const FString QueryName = TEXT("createPerformance");

	FDataObjectBuilder DataObjectBuilder = FDataObjectBuilder();
	DataObjectBuilder.AddUStruct(PerformanceCreateInput);
	DataObjectBuilder.AddConnect("owner", OwnerWhereUniqueInput);
	
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

void UPerformanceRepository::GetAllPerformances(
	const TFunction<void(const TArray<FCMSPerformance>& Performances)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("performances");
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query GetAllPerformances {
      %s {
		%s
      }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);
	ExecuteGraphQLQuery<FCMSPerformance>(
		Query,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UPerformanceRepository::FindPerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("performance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	query FindPerformance($where: PerformanceWhereUniqueInput!) {
      %s (where: $where) {
		%s
      }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);
	
	const TSharedPtr<FJsonObject> WhereObject = MakeShareable(new FJsonObject());
	WhereObject->SetStringField("id", Where.Id);
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeShared<FJsonValueObject>(WhereObject)}
	};
	
	ExecuteGraphQLQuery<FCMSPerformance>(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UPerformanceRepository::DeletePerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation DeletePerformance($where: PerformanceWhereUniqueInput!) {
      deletePerformance(where: $where) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*GQLPerformance
	);
	
	const TSharedPtr<FJsonObject> WhereObject = MakeShareable(new FJsonObject());
	WhereObject->SetStringField("id", Where.Id);
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeShared<FJsonValueObject>(WhereObject)}
	};
	const FString QueryName = TEXT("deletePerformance");
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UPerformanceRepository::UpdatePerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	const FCMSPerformanceUpdateInput& Data,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation UpdatePerformance($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
      %s (where: $where, data: $data) {
		%s
      }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);
	
	const TSharedPtr<FJsonObject> WhereObject = MakeShareable(new FJsonObject());
	WhereObject->SetStringField("id", Where.Id);
	const TSharedPtr<FJsonObject> DataObject = MakeShareable(new FJsonObject());
	DataObject->SetStringField("title", Data.Title);
	DataObject->SetStringField("about", Data.About);
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeShared<FJsonValueObject>(WhereObject)},
		{"data", MakeShared<FJsonValueObject>(DataObject)}
	};
	
	ExecuteGraphQLQuery<FCMSPerformance>(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UPerformanceRepository::ConnectUsdScenes(
	const FCMSIdInput& PerformanceWhereUniqueInput,
	const TArray<FCMSIdInput>& UsdScenesToConnectUsdSceneWhereUniqueInputs,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectUsdScenesToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddConnect("usdScenes", UsdScenesToConnectUsdSceneWhereUniqueInputs);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhereUniqueInput)},
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

void UPerformanceRepository::ConnectSessions(const FCMSIdInput& PerformanceWhereUniqueInput,
	const TArray<FCMSIdInput>& SessionsScenesToConnectUsdSceneWhereUniqueInputs,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectUsdScenesToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddConnect("sessions", SessionsScenesToConnectUsdSceneWhereUniqueInputs);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhereUniqueInput)},
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

void UPerformanceRepository::DisconnectSessions(const FCMSIdInput& PerformanceWhereUniqueInput,
	const TArray<FCMSIdInput>& SessionsWhere,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectUsdScenesToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddDisconnect("sessions", SessionsWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhereUniqueInput)},
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

void UPerformanceRepository::RemoveUsdScene(
	const FCMSUsdScenePerformanceWhereInput& Where,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("removeUsdSceneFromPerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation RemoveUsdSceneFromPerformance ($where: UsdScenePerformanceWhereInput!) {
	  %s (where: $where) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(Where)}
	};
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UPerformanceRepository::ConnectMembers(
	const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	const TArray<FCMSUserWhereUniqueInput>& MembersWhere,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectMembersToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddConnect("members", MembersWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhere)},
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

void UPerformanceRepository::DisconnectMembers(
	const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	const TArray<FCMSUserWhereUniqueInput>& MembersWhere,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectMembersToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddDisconnect("members", MembersWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhere)},
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

void UPerformanceRepository::ConnectAvatars(
	const FCMSIdInput& PerformanceWhere,
	const TArray<FCMSIdInput>& AvatarsWhere,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectAvatarToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddConnect("avatars", AvatarsWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhere)},
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

void UPerformanceRepository::DisconnectAvatars(
	const FCMSIdInput& PerformanceWhere,
	const TArray<FCMSIdInput>& AvatarsWhere,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updatePerformance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectAvatarToPerformance ($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPerformanceFragments,
	*QueryName,
	*GQLPerformance
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddDisconnect("avatars", AvatarsWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(PerformanceWhere)},
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
