
#include "Repositories/UsdSceneRepository.h"

#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

void UUsdSceneRepository::Create(
	const FCMSPerformanceCreateInput& PerformanceCreateInput,
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
	*GQLUserFragment,
	*GQLPerformance
	);
	const FString QueryName = TEXT("createPerformance");

	const TSharedPtr<FJsonObject> DataObject = MakeShareable(new FJsonObject());
	DataObject->SetStringField("title", PerformanceCreateInput.Title);
	DataObject->SetStringField("ownerId", PerformanceCreateInput.OwnerId);
	DataObject->SetStringField("about", PerformanceCreateInput.About);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"data", MakeShared<FJsonValueObject>(DataObject)}
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
	const FCMSPerformanceWhereUniqueInput& Where,
	const FCMSPerformanceUpdateInput& Data,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure) const
{
	const FString QueryName = TEXT("performance");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation UpdatePerformance($where: PerformanceWhereUniqueInput!, $data: PerformanceUpdateInput!) {
      %s (where: $where, data: $data) {
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
