
#include "Repositories/SessionRepository.h"

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
		state
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

void USessionRepository::GetActiveSessions(
	FOnGetActiveSessionsSuccess OnSuccess,
	FOnFailure OnFailure
) const
{
	const FString Query = TEXT(R"(
	query GetSesessionByState ($state: String) {
	  sessionByState (state: $state) {
		id
		title
		state
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
		{"state", "active" }
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
		if (
			TArray<FCMSSession> Sessions;
			ParseCMSMultipleSessionsFromResponse(ResponseContent, TEXT("sessionByState"), Sessions, ErrorReason))
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("Successfully parsed response to CMSSession"));
			OnSuccess.ExecuteIfBound(Sessions);
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response to CMSSession. Reason: %s"), *ErrorReason);
		}
	});
	DataSource->ExecuteGraphQLQuery(Query, QueryParams, OnResponse);
}

bool USessionRepository::ParseCMSMultipleSessionsFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	TArray<FCMSSession>& OutSessions,
	FString& OutErrorReason
)
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

	const TArray<TSharedPtr<FJsonValue>>* SessionsArray;
	if (!DataObject->TryGetArrayField(QueryName, SessionsArray))
	{
		OutErrorReason = FString::Printf(TEXT("%s array is invalid or missing"), *QueryName);
		return false;
	}

	for (const TSharedPtr<FJsonValue>& SessionValue : *SessionsArray)
	{
		const TSharedPtr<FJsonObject> SessionObject = SessionValue->AsObject();
		if (!SessionObject.IsValid())
		{
			OutErrorReason = TEXT("Invalid session object in array");
			return false;
		}

		FCMSSession ParsedSession;
		if (!CreateSessionFromSingleSessionJsonObject(SessionObject, ParsedSession, OutErrorReason))
		{
			return false; // Error details are already set in OutErrorReason
		}

		OutSessions.Add(ParsedSession);
	}

	return true;
}

void USessionRepository::CreateSession(
	const FCMSSession& InSession,
	FOnGetSessionSuccess OnSuccess,
	FOnFailure OnFailure
) const
{
	const FString Query = TEXT(R"(
	mutation CreateSession(
		$title: String!, 
		$ownerId: ID!, 
		$performanceId: ID!, 
		$state: String!, 
		$streamingUrl: String = "", 
		$eosSessionId: String = "", 
		$audioDataId: ID
	  ) {
	  createSession(
		title: $title,
		ownerId: $ownerId,
		performanceId: $performanceId,
		state: $state,
		streamingUrl: $streamingUrl,
		eosSessionId: $eosSessionId,
		audioDataId: $audioDataId
	  ) {
		id
		eosSessionId
		title
		state
		streamingUrl
		audioData {
		  id
		}
		performance {
		  id
		}
		owner {
		  id
		}
	  }
	}
	)");

	const TMap<FString, FString> QueryParams = {
		{"eosSessionId", InSession.EosSessionId },
		{"title", InSession.Title },
		{"ownerId", InSession.OwnerId },
		{"performanceId", InSession.PerformanceId },
		{"state", InSession.State },
		{"streamingUrl", InSession.StreamingUrl },
		{"audioDataId", InSession.AudioDataId },
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
		FCMSSession Session;
		if (ParseCMSSessionFromResponse(ResponseContent, TEXT("createSession"), Session, ErrorReason))
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

bool USessionRepository::CreateSessionFromSingleSessionJsonObject(
	const TSharedPtr<FJsonObject>& SessionJsonObject,
	FCMSSession& OutSession,
	FString& OutErrorReason
)
{
	OutSession.Id = SessionJsonObject->GetStringField(TEXT("id"));
	OutSession.EosSessionId = SessionJsonObject->GetStringField(TEXT("eosSessionId"));
	OutSession.Title = SessionJsonObject->GetStringField(TEXT("title"));
	OutSession.StreamingUrl = SessionJsonObject->GetStringField(TEXT("streamingUrl"));
	OutSession.State = SessionJsonObject->GetStringField(TEXT("state"));

	if (
		const TSharedPtr<FJsonObject> AudioDataObject = SessionJsonObject->GetObjectField(TEXT("audioData"));
		AudioDataObject.IsValid()
	)
	{
		OutSession.AudioDataId = AudioDataObject->GetStringField(TEXT("id"));
	}
	else
	{
		OutErrorReason = TEXT("audioData invalid");
		return false;
	}
		
	if (
		const TSharedPtr<FJsonObject> PerformanceObject = SessionJsonObject->GetObjectField(TEXT("performance"));
		PerformanceObject.IsValid()
	)
	{
		OutSession.PerformanceId = PerformanceObject->GetStringField(TEXT("id"));
	}
	else
	{
		OutErrorReason = TEXT("performance invalid");
		return false;
	}
	
	if (
		const TSharedPtr<FJsonObject> OwnerObject = SessionJsonObject->GetObjectField(TEXT("owner"));
		OwnerObject.IsValid()
	)
	{
		OutSession.OwnerId = OwnerObject->GetStringField(TEXT("id"));
	}
	else
	{
		OutErrorReason = TEXT("owner invalid");
		return false;
	}
	
	return true;
}

bool USessionRepository::ParseCMSSessionFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	FCMSSession& OutSession,
	FString& OutErrorReason
)
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
		OutErrorReason = TEXT("sessionById invalid");
		return false;
	};

	if (!CreateSessionFromSingleSessionJsonObject(SessionByIdObject, OutSession, OutErrorReason))
	{
		return false;
	}
		
	return true;
}
