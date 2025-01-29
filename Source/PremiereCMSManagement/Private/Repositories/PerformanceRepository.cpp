
#include "Repositories/PerformanceRepository.h"

#include "LogPremiereCMSManagement.h"
#include "GraphQLDataSource.h"
#include "Structs/CMSSession.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

void UPerformanceRepository::Initialize(UGraphQLDataSource* InDataSource)
{
	DataSource = InDataSource;
}

void UPerformanceRepository::CreatePerformance(
	const FCMSPerformanceCreateInput& PerformanceCreateInput,
	FOnGetPerformanceSuccess OnSuccess,
	FOnFailure OnFailure
)
{
	const FString Query = TEXT(R"(
	mutation CreatePerformanceMutation($data: PerformanceCreateInput!) {
	  createPerformance(data: $data) {
		id
		title
		about
		owner {
		  id
		}
		members {
		  id
		}
		usdScenes {
		  id
		}
		sessions {
		  id
		}
		avatars {
		  id
		}
	  }
	}
	)");


	const TSharedPtr<FJsonObject> DataObject = MakeShareable(new FJsonObject());
	DataObject->SetStringField("title", PerformanceCreateInput.Title);
	DataObject->SetStringField("ownerId", PerformanceCreateInput.OwnerId);
	DataObject->SetStringField("about", PerformanceCreateInput.About);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"data", MakeShared<FJsonValueObject>(DataObject)}
	};
	
	FOnGraphQLResponse OnResponse;
	OnResponse.BindLambda([OnSuccess, OnFailure](const FGraphQLResult GraphQLResult)
	{
		const bool bSuccess = GraphQLResult.GraphQLOutcome == Success;
		const FString ResponseContent = GraphQLResult.RawResponse;
		
		if (!bSuccess)
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("GraphQL API Error. ErrorMessage: %s"), *GraphQLResult.ErrorMessage);
			OnFailure.ExecuteIfBound(GraphQLResult.ErrorMessage);
			return;
		}
		UE_LOG(LogPremiereCMSManagement, Log, TEXT("GraphQL request succeeded. Response: %s"), *ResponseContent);
		
		FString ErrorReason;
		FCMSPerformance Performance;
		if (ParseCMSObjectFromResponse(ResponseContent, TEXT("createPerformance"), Performance, ErrorReason))
		{
			OnSuccess.ExecuteIfBound(Performance);
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response to CMSPerformance. Reason: %s"), *ErrorReason);
		}
	});
	DataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
	
}

bool UPerformanceRepository::ParseCMSObjectFromResponse(const FString& JsonResponse, const FString& QueryName,
	FCMSPerformance& OutPerformance, FString& OutErrorReason)

{
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);
	TSharedPtr<FJsonObject> JsonObject;
	if (
		!(FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	)
	{
		OutErrorReason = TEXT("could not deserialize json object");
		return false;
	}
	
	const TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(TEXT("data"));
	if (!DataObject.IsValid())
	{
		OutErrorReason = TEXT("data invalid");
		return false;
	}

	const TSharedPtr<FJsonObject> SessionByIdObject = DataObject->GetObjectField(QueryName);
	if (!SessionByIdObject.IsValid())
	{
		OutErrorReason = FString::Printf(TEXT("%s invalid"), *QueryName);
		return false;
	};

	if (!CreatePerformanceFromSingleUserJsonObject(SessionByIdObject, OutPerformance, OutErrorReason))
	{
		return false;
	}
		
	return true;
}

bool UPerformanceRepository::CreatePerformanceFromSingleUserJsonObject(
	const TSharedPtr<FJsonObject>& SessionJsonObject,
	FCMSPerformance& OutPerformance,
	FString& OutErrorReason
)
{
	OutPerformance.Id = SessionJsonObject->GetStringField(TEXT("id"));
	OutPerformance.OwnerId = SessionJsonObject->GetStringField(TEXT("ownerId"));
	OutPerformance.Title = SessionJsonObject->GetStringField(TEXT("title"));
	return true;
}
