
#include "Repositories/UsdSceneRepository.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

void UUsdSceneRepository::Create(
	const FCMSUsdSceneCreateInput& UsdSceneCreateInput,
	const TFunction<void(const FCMSUsdScene& UsdScene)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("createUsdScene");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation CreatePerformanceMutation ($data: UsdSceneCreateInput!) {
	  %s (data: $data) {
		%s
      }
	}
	)"),
	*GQLUsdSceneFragments,
	*QueryName,
	*GQLUsdScene
	);

	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"data", MakeDataValue(UsdSceneCreateInput)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UUsdSceneRepository::GetAll(
	const TFunction<void(const TArray<FCMSUsdScene>& UsdScenes)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("usdScenes");
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query GetAll {
      %s {
		%s
      }
	}
	)"),
	*GQLUsdSceneFragments,
	*QueryName,
	*GQLUsdScene
	);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Query);
	ExecuteGraphQLQuery<FCMSUsdScene>(
		Query,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UUsdSceneRepository::Find(
	const FCMSPerformanceWhereUniqueInput& Where,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("performance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	query Find ($where: PerformanceWhereUniqueInput!) {
      %s (where: $where) {
		%s
      }
	}
	)"),
	*GQLUserFragment,
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

void UUsdSceneRepository::Delete(
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
	*GQLUserFragment,
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

void UUsdSceneRepository::Update(
	const FCMSUsdSceneWhereUniqueInput& Where,
	const FCMSUsdSceneUpdateInput& Data,
	const TFunction<void(const FCMSUsdScene& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("updateUsdScene");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation UpdatePerformance($where: UsdSceneWhereUniqueInput!, $data: UsdSceneUpdateInput!) {
      %s (where: $where, data: $data) {
		%s
      }
	}
	)"),
	*GQLUsdSceneFragments,
	*QueryName,
	*GQLUsdScene
	);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(Where)},
		{"data", MakeDataValue(Data)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}
