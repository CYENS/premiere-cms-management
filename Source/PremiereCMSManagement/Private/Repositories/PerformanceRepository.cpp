
#include "Repositories/PerformanceRepository.h"

#include "LogPremiereCMSManagement.h"
#include "GraphQLDataSource.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Containers/Array.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

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
          name
          email
          eosId
          userRole
          isAdmin
          isSuperAdmin
          createdAt
          person {
            id
            givenName
            familyName
            artisticName
          }
          performances {
            id
          }
          avatars {
            id
          }
          sessionsOwned {
            id
          }
          sessionAttendance {
            id
          }
        }
        members {
          id
          name
          email
          eosId
          userRole
          isAdmin
          isSuperAdmin
          createdAt
          person {
            id
            givenName
            familyName
            artisticName
          }
          performances {
            id
          }
          avatars {
            id
          }
          sessionsOwned {
            id
          }
          sessionAttendance {
            id
          }
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

void UPerformanceRepository::GetAllPerformances(FOnGetPerformancesSuccess OnSuccess, FOnFailure OnFailure) const
{
	const FString Query = TEXT(R"(
    query GetAllPerformances {
      performances {
        id
        title
        about
        owner {
          id
          name
          email
          eosId
          userRole
          isAdmin
          isSuperAdmin
          createdAt
          person {
            id
            givenName
            familyName
            artisticName
          }
          performances {
            id
          }
          avatars {
            id
          }
          sessionsOwned {
            id
          }
          sessionAttendance {
            id
          }
        }
        members {
          id
          name
          email
          eosId
          userRole
          isAdmin
          isSuperAdmin
          createdAt
          person {
            id
            givenName
            familyName
            artisticName
          }
          performances {
            id
          }
          avatars {
            id
          }
          sessionsOwned {
            id
          }
          sessionAttendance {
            id
          }
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

	FOnGraphQLResponse OnResponse;
	OnResponse.BindLambda([OnSuccess, OnFailure](const FGraphQLResult GraphQLResult)
	{
		const bool bSuccess = GraphQLResult.GraphQLOutcome == Success;
		const FString ResponseContent = GraphQLResult.RawResponse;
		
		if (!bSuccess)
		{
			const FString ErrorMessage = GraphQLResult.ErrorMessage;
			OnFailure.ExecuteIfBound(ErrorMessage);
			return;
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("GraphQL request succeeded. Response: %s"), *ResponseContent);
		}
		
		FString ErrorReason;
		if (
			TArray<FCMSPerformance> Performances;
			ParsePerformanceArrayFromResponse(ResponseContent, TEXT("performances"), Performances, ErrorReason))
		{
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("Successfully parsed response %d"), Performances.Num());
			UE_LOG(LogPremiereCMSManagement, Log, TEXT("Successfully parsed response"));
			OnSuccess.ExecuteIfBound(Performances);
		}
		else
		{
			UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response. Reason: %s"), *ErrorReason);
		}
	});
	DataSource->ExecuteGraphQLQuery(Query, OnResponse);
}

void UPerformanceRepository::FindPerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	const TFunction<void(const FCMSPerformance& Performance)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString Query = TEXT(R"(
	query FindPerformance($where: PerformanceWhereUniqueInput!) {
      performance(where: $where) {
        id
        title
        about
        owner {
          id
          name
          email
          eosId
          userRole
          isAdmin
          isSuperAdmin
          createdAt
          person {
            id
            givenName
            familyName
            artisticName
          }
          performances {
            id
          }
          avatars {
            id
          }
          sessionsOwned {
            id
          }
          sessionAttendance {
            id
          }
        }
        members {
          id
          name
          email
          eosId
          userRole
          isAdmin
          isSuperAdmin
          createdAt
          person {
            id
            givenName
            familyName
            artisticName
          }
          performances {
            id
          }
          avatars {
            id
          }
          sessionsOwned {
            id
          }
          sessionAttendance {
            id
          }
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
	
	const TSharedPtr<FJsonObject> WhereObject = MakeShareable(new FJsonObject());
	WhereObject->SetStringField("id", Where.Id);
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeShared<FJsonValueObject>(WhereObject)}
	};
	const FString QueryName = TEXT("performance");
	ExecuteGraphQLQuery<FCMSPerformance>(
		Query,
		Variables,
		QueryName,
		[](const FGraphQLResult& GraphQLResult, const FString& QueryName, FCMSPerformance& OutPerformance, FString& OutErrorReason)
		{
			
			const TSharedPtr<FJsonObject> ResponseObject = GraphQLResult.ResponseObject->GetObjectField("data")->GetObjectField(QueryName);
			return ParsePerformanceFromJsonObject(
				ResponseObject,
				OutPerformance,
				OutErrorReason
			);
		},
		OnSuccess,
		OnFailure
	);
}

bool UPerformanceRepository::ParsePerformanceArrayFromResponse(
	const FString& ResponseContent,
	const FString& QueryName,
	TArray<FCMSPerformance>& OutPerformances,
	FString& OutErrorReason
)
{
	return ParseArrayOfItemsFromResponse<FCMSPerformance>(
		ResponseContent,
		QueryName,
		OutPerformances,
		[&](const TSharedPtr<FJsonObject>& JsonObject, FCMSPerformance& Performance, FString& Error) -> bool
		{
			return ParsePerformanceFromJsonObject(JsonObject, Performance, Error);
		},
		OutErrorReason
	);
}

bool UPerformanceRepository::ParseCMSObjectFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	FCMSPerformance& OutPerformance,
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
		OutErrorReason = FString::Printf(TEXT("%s invalid"), *QueryName);
		return false;
	};

	if (!ParsePerformanceFromJsonObject(SessionByIdObject, OutPerformance, OutErrorReason))
	{
		return false;
	}
		
	return true;
}

bool UPerformanceRepository::ParsePerformanceFromJsonObject(
	const TSharedPtr<FJsonObject>& JsonObject,
	FCMSPerformance& OutPerformance,
	FString& OutErrorReason
)
{
	FJsonObjectConverter::JsonObjectToUStruct<FCMSPerformance> (
		JsonObject.ToSharedRef(),
		&OutPerformance,
		0,
		0
	);
	OutPerformance.OwnerId = OutPerformance.Owner.Id;
	return true;
}
