
#include "Repositories/UserRepository.h"

#include "Structs/CMSTypes.h"
#include "LogPremiereCMSManagement.h"
#include "GraphQLDataSource.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UUserRepository::GetAll(
	const TFunction<void(const TArray<FCMSUser>& Users)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("users");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	query GetAllUsers {
	  %s {
		%s
	  }
	}
	)"),
	*GQLUserFragments,
	*QueryName,
	*GQLUser
	);
	ExecuteGraphQLQuery<FCMSUser>(
		Query,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void UUserRepository::Find(
	const FCMSIdInput& Where,
	const TFunction<void(const FCMSUser& User)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("user");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	query Find ($where: UserWhereUniqueInput!) {
	  %s(where: $where) {
		%s
	  }
	}
	)"),
	*GQLUserFragments,
	*QueryName,
	*GQLUser
	);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(Where)}
	};
	
	ExecuteGraphQLQuery<FCMSUser>(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

bool UUserRepository::ParseMultipleCMSUsersFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	TArray<FCMSUser>& OutUsers,
	FString& OutErrorReason
)
{
	return ParseArrayOfItemsFromResponse<FCMSUser>(
		JsonResponse,
		QueryName,
		OutUsers,
		[&](const TSharedPtr<FJsonObject>& Obj, FCMSUser& User, FString& Error) -> bool
		{
			return ParseCMSUserFromSingleUserJsonObject(Obj, User, Error);
		},
		OutErrorReason
	);
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

	if (!ParseCMSUserFromSingleUserJsonObject(SessionByIdObject, OutUser, OutErrorReason))
	{
		return false;
	}
		
	return true;
}

bool UUserRepository::ParseCMSUserFromSingleUserJsonObject(
	const TSharedPtr<FJsonObject>& SessionJsonObject,
	FCMSUser& OutUser,
	FString& OutErrorReason
)
{
	FText OutReason = FText::FromString(*OutErrorReason);
	FJsonObjectConverter::JsonObjectToUStruct<FCMSUser>(
		SessionJsonObject.ToSharedRef(),
		&OutUser,
		0,
		0,
		false,
		&OutReason
	);
	OutErrorReason = OutReason.ToString();
	SessionJsonObject->GetObjectField("person")->GetStringField(TEXT("id"));

	OutUser.Id = SessionJsonObject->GetStringField(TEXT("id"));
	OutUser.EosId = SessionJsonObject->GetStringField(TEXT("EosId"));
	OutUser.Name = SessionJsonObject->GetStringField(TEXT("name"));
	OutUser.Email = SessionJsonObject->GetStringField(TEXT("email"));
	OutUser.UserRole = SessionJsonObject->GetStringField(TEXT("userRole"));
	OutUser.IsSuperAdmin = SessionJsonObject->GetBoolField(TEXT("isSuperAdmin"));

	return true;
}
