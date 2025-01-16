
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
	query Query($id: ID) {
	  sessionById(id: $id) {
		id
		title
		streamingUrl
		audioData {
			id
			fileUrl
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
		if (FCMSSession Session; ParseCMSSessionFromResponse(ResponseContent, Session, ErrorReason))
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

		const TSharedPtr<FJsonObject> SessionByIdObject = DataObject->GetObjectField(TEXT("sessionById"));
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
		return true;
	}

	return false;
}
