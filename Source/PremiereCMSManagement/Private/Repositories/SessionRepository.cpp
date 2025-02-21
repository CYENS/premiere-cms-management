
#include "Repositories/SessionRepository.h"

#include "DataObjectBuilder.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

void USessionRepository::GetAll(
	const TFunction<void(const TArray<FCMSSession>& Sessions)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("sessions");
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query GetAll {
      %s {
		%s
      }
	}
	)"),
	*GQLSessionFragments,
	*QueryName,
	*GQLSession
	);
	ExecuteGraphQLQuery(
		Query,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void USessionRepository::FindSession(
	const FCMSSessionWhereUniqueInput& Where,
	const TFunction<void(const FCMSSession& Session)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("session");
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query FindSession($where: SessionWhereUniqueInput!){
      %s (where: $where) {
		%s
      }
	}
	)"),
	*GQLSessionFragments,
	*QueryName,
	*GQLSession
	);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(Where)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
        QueryName,
		OnSuccess,
		OnFailure
	);
}

void USessionRepository::GetActiveSessions(
	const TFunction<void(const TArray<FCMSSession>& Sessions)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("sessions");
	const FString Query = FString::Printf(
		TEXT(R"(
		%s
		query GetActiveSessions ($where: SessionWhereInput!) {
		  %s (where: $where) {
			%s
		  }
		}
		)"),
		*GQLSessionFragments,
		*QueryName,
		*GQLSession
	);

	const FString Variables = TEXT(R"(
	{
	  "where": {
		"state": {
		  "name": {
			"equals": "active"
		  }
		}
	  }
	}
	)");

	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

void USessionRepository::CreateSession(
	const FCMSSessionCreateInput& Data,
	const FCMSIdInput& SessionStateWhereId,
	const TOptional<FCMSIdInput>& OwnerWhere,
	const TOptional<FCMSIdInput>& UsdSceneWhere,
	const TArray<FCMSIdInput>& AudioDataWhereIds,
	const TArray<FCMSIdInput>& FaceDataWhereIds,
	const TFunction<void(const FCMSSession& Sessions)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = TEXT("createSession");

	const FString Query = FString::Printf(
		TEXT(R"(
		%s
		mutation Create ($data: SessionCreateInput!) {
			%s (data: $data) {
				%s
			}
		}
		)"),
		*GQLSessionFragments,
		*QueryName,
		*GQLSession
	);

	FDataObjectBuilder ObjectBuilder;
	ObjectBuilder.AddConnect("state", SessionStateWhereId);
	ObjectBuilder.AddConnect("faceData", FaceDataWhereIds);
	ObjectBuilder.AddConnect("audioData", AudioDataWhereIds);
	ObjectBuilder.AddConnect("owner", OwnerWhere);
	ObjectBuilder.AddConnect("scene", UsdSceneWhere);
	ObjectBuilder.AddUStruct(Data);

	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{
			"data", ObjectBuilder.BuildAsJsonValue()
		}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}
