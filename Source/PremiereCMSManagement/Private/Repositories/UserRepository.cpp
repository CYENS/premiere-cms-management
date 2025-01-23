
#include "Repositories/UserRepository.h"

#include "Structs/CMSUser.h"
#include "LogPremiereCMSManagement.h"
#include "GraphQLDataSource.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UUserRepository::Initialize(UGraphQLDataSource* InDataSource)
{
	DataSource = InDataSource;
}

void UUserRepository::CreateUser(
	const FCMSUser& InUser,
	FOnGetUserSuccess OnSuccess,
	FOnFailure OnFailure
) const
{
	const FString Query = TEXT(R"(
	mutation CreateUser($data: UserCreateInput!) {
	  createUser(data: $data) {
		id
		eosId
		name
		email
		userRole
		isAdmin
		performances {
		  id
		}
		avatars {
		  id
		}
		sessionsOwned {
		  id
		}
		sessionsAttending {
		  id
		}
	  }
	}
	)");


	const TSharedPtr<FJsonObject> DataObject = MakeShareable(new FJsonObject());
	DataObject->SetStringField("eosId", InUser.EosId);
	DataObject->SetStringField("email", InUser.Email);
	DataObject->SetStringField("name", InUser.Name);
	DataObject->SetStringField("userRole", InUser.UserRole);
	DataObject->SetBoolField("isSuperAdmin", InUser.IsSuperAdmin);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"data", MakeShared<FJsonValueObject>(DataObject)}
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
		FCMSUser User;
		if (ParseCMSUserFromResponse(ResponseContent, TEXT("createUser"), User, ErrorReason))
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("Successfully parsed response to CMSSession"));
			OnSuccess.ExecuteIfBound(User);
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response to CMSSession. Reason: %s"), *ErrorReason);
		}
	});
	DataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
}

bool UUserRepository::ParseCMSUserFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	FCMSUser& OutUser,
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

	if (!CreateSessionFromSingleUserJsonObject(SessionByIdObject, OutUser, OutErrorReason))
	{
		return false;
	}
		
	return true;
}

bool UUserRepository::CreateSessionFromSingleUserJsonObject(
	const TSharedPtr<FJsonObject>& SessionJsonObject,
	FCMSUser& OutUser,
	FString& OutErrorReason
)
{
	OutUser.Id = SessionJsonObject->GetStringField(TEXT("id"));
	OutUser.EosId = SessionJsonObject->GetStringField(TEXT("EosId"));
	OutUser.Name = SessionJsonObject->GetStringField(TEXT("name"));
	OutUser.Email = SessionJsonObject->GetStringField(TEXT("email"));
	OutUser.UserRole = SessionJsonObject->GetStringField(TEXT("userRole"));
	OutUser.IsSuperAdmin = SessionJsonObject->GetBoolField(TEXT("isSuperAdmin"));

	return true;
}
