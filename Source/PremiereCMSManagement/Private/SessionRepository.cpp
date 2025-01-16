
#include "SessionRepository.h"

#include "LogPremiereCMSManagement.h"
#include "GraphQLDataSource.h"
#include "Structs/CMSSession.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void USessionRepository::Initialize(UGraphQLDataSource* InDataSource)
{
	DataSource = InDataSource;
}

void USessionRepository::GetSessionById(
	const FString& SessionId,
	FOnGetSessionSuccess OnSuccess,
	FOnFailure OnFailure
) const
{
	const FString Query = TEXT(R"(
	query GetSessionById($id: ID) {
	  sessionById(id: $id) {
		id
		title
		streamingUrl
		audioData {
			id
			fileUrl
		}
		performance {
			id
			title
			description
		}
	  }
	}
	)");

	const TMap<FString, FString> QueryParams = {
		{"id", SessionId }
	};
	
	FOnGraphQLResponse OnResponse;
	OnResponse.BindLambda([OnSuccess, OnFailure](const bool bSuccess, const FString& ResponseContent)
	{
		if (!bSuccess)
		{
			const FString ErrorMessage = FString::Printf(TEXT("GraphQL request failed. Response: %s"), *ResponseContent);
			OnFailure.ExecuteIfBound(ErrorMessage);
			return;
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("GraphQL request succeeded. Response: %s"), *ResponseContent);
		}
		
		FString ErrorReason;
		if (FCMSSession Session; ParseCMSSessionFromResponse(ResponseContent, TEXT("sessionById"), Session, ErrorReason))
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("Successfully parsed response to CMSSession"));
			OnSuccess.ExecuteIfBound(Session);
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response to CMSSession. Reason: %s"), *ErrorReason);
		}
	});
	DataSource->ExecuteGraphQLQuery(Query, QueryParams, OnResponse);
}

void USessionRepository::CreateSession(
	const FString& Title,
	const FString& OwnerId,
	const FString& PerformanceId,
	FOnGetSessionSuccess OnSuccess,
	FOnFailure OnFailure
) const
{
	const FString Query = TEXT(R"(
	mutation CreateSession($title: String!, $ownerId: ID!, $performanceId: ID!) {
	  createSession(title: $title, ownerId: $ownerId, performanceId: $performanceId) {
		id
		title
		streamingUrl
		audioData {
			id
			fileUrl
		}
		performance {
			id
			title
			description
		}
	  }
	}
	)");

	const TMap<FString, FString> QueryParams = {
		{"title", Title },
		{"ownerId", OwnerId },
		{"performanceId", PerformanceId },
	};
	
	FOnGraphQLResponse OnResponse;
	OnResponse.BindLambda([OnSuccess, OnFailure](const bool bSuccess, const FString& ResponseContent)
	{
		if (!bSuccess)
		{
			const FString ErrorMessage = FString::Printf(TEXT("GraphQL request failed. Response: %s"), *ResponseContent);
			OnFailure.ExecuteIfBound(ErrorMessage);
			return;
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("GraphQL request succeeded. Response: %s"), *ResponseContent);
		}
		
		FString ErrorReason;
		if (FCMSSession Session; ParseCMSSessionFromResponse(ResponseContent, TEXT("createSession"), Session, ErrorReason))
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("Successfully parsed response to CMSSession"));
			OnSuccess.ExecuteIfBound(Session);
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response to CMSSession. Reason: %s"), *ErrorReason);
		}
	});
	DataSource->ExecuteGraphQLQuery(Query, QueryParams, OnResponse);
}

bool USessionRepository::ParseCMSSessionFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	FCMSSession& OutSession,
	FString& OutErrorReason
)
{
	const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);
	if (
		TSharedPtr<FJsonObject> JsonObject;
		FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid()
	)
	{
		const TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(TEXT("data"));
		if (!DataObject.IsValid())
		{
			OutErrorReason = TEXT("data invalid");
			return false;
		}

		const TSharedPtr<FJsonObject> SessionByIdObject = DataObject->GetObjectField(QueryName);
		if (!SessionByIdObject.IsValid())
		{
			OutErrorReason = TEXT("sessionById invalid");
			return false;
		};

		OutSession.Id = SessionByIdObject->GetStringField(TEXT("id"));
		OutSession.Title = SessionByIdObject->GetStringField(TEXT("title"));
		OutSession.StreamingUrl = SessionByIdObject->GetStringField(TEXT("streamingUrl"));

		if (
			const TSharedPtr<FJsonObject> AudioDataObject = SessionByIdObject->GetObjectField(TEXT("audioData"));
			AudioDataObject.IsValid()
		)
		{
			OutSession.AudioDataId = AudioDataObject->GetStringField(TEXT("id"));
			OutSession.AudioDataFileUrl = AudioDataObject->GetStringField(TEXT("fileUrl"));
		}
		else
		{
			OutErrorReason = TEXT("audioData invalid");
		}
		
		if (
			const TSharedPtr<FJsonObject> PerformanceObject = SessionByIdObject->GetObjectField(TEXT("performance"));
			PerformanceObject.IsValid()
		)
		{
			OutSession.PerformanceId = PerformanceObject->GetStringField(TEXT("id"));
			OutSession.PerformanceTitle = PerformanceObject->GetStringField(TEXT("title"));
			OutSession.PerformanceDescription = PerformanceObject->GetStringField(TEXT("description"));
		}
		else
		{
			OutErrorReason = TEXT("performance invalid");
		}
		
		return true;
	}

	return false;
}
