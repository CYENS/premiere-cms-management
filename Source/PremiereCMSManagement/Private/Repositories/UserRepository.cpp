
#include "Repositories/UserRepository.h"

#include "DataObjectBuilder.h"
#include "EditorCategoryUtils.h"
#include "Structs/CMSTypes.h"
#include "Structs/CMSInputs.h"
#include "LogPremiereCMSManagement.h"
#include "GraphQLDataSource.h"
#include "JsonObjectConverter.h"
#include "Repositories/GraphQLConstants.h"
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

void UUserRepository::Create(
	const FCMSUserCreateInput& Data,
	const TOptional<FCMSIdInput>& PersonWhereId,
	const TFunction<void(const FCMSUser& User)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("createUser");
	const FString Query = FString::Printf(
		TEXT(R"(
	%s
	mutation CreateUser($data: UserCreateInput!) {
	  %s(data: $data) {
		%s
	  }
	}
	)"),
	*GQLUserFragments,
	*QueryName,
	*GQLUser
	);

	FDataObjectBuilder DataObjectBuilder = FDataObjectBuilder();
	DataObjectBuilder.AddUStruct(Data);
	if (PersonWhereId.IsSet())
	{
		DataObjectBuilder.AddConnect("person", PersonWhereId.GetValue());
	}
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{
		"data", DataObjectBuilder.BuildAsJsonValue()
		}
	};
	UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *Query);
	ExecuteGraphQLQuery(Query, Variables, QueryName, OnSuccess, OnFailure);
}

void UUserRepository::Update(
	const FCMSIdInput& Where,
	const FCMSPerformanceUpdateInput& Data,
	const TFunction<void(const FCMSUser& User)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
}

void UUserRepository::ConnectPerson(
	const FCMSUserWhereUniqueInput& UserWhere,
	const FCMSPersonWhereUniqueInput& PersonWhere,
	const TFunction<void(const FCMSUser& User)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
)
{
	const FString QueryName = TEXT("updateUser");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation ConnectPersonToUser ($where: UserWhereUniqueInput!, $data: UserUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLUserFragments,
	*QueryName,
	*GQLUser
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddConnect("person", PersonWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(UserWhere)},
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

void UUserRepository::DisconnectPerson(
	const FCMSUserWhereUniqueInput& UserWhere,
	const FCMSPersonWhereUniqueInput& PersonWhere,
	const TFunction<void(const FCMSUser& User)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
)
{
	const FString QueryName = TEXT("updatePerson");
	const FString Query = FString::Printf(TEXT(R"(
	%s
	mutation DisconnectAvatarFromPerformance ($where: PersonWhereUniqueInput!, $data: PersonUpdateInput!) {
	  %s (where: $where, data: $data) {
		%s
	  }
	}
	)"),
	*GQLPersonFragments,
	*QueryName,
	*GQLPerson
	);

	FDataObjectBuilder DataObjectBuilder;
	DataObjectBuilder.AddDisconnect("person", PersonWhere);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(UserWhere)},
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
