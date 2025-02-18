
#include "PremiereCMSManagementSubsystem.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "PremiereCMSDeveloperSettings.h"
#include "Repositories/PerformanceRepository.h"
#include "Repositories/SessionRepository.h"
#include "Repositories/UsdSceneRepository.h"
#include "Repositories/UserRepository.h"
#include "Structs/CMSInputs.h"

UPremiereCMSManagementSubsystem::UPremiereCMSManagementSubsystem()
{
	DeveloperSettings = GetMutableDefault<UPremiereCMSDeveloperSettings>();
	
	GraphQLUrl = DeveloperSettings->GraphQLUrl;
	
	GraphQlDataSource = NewObject<UGraphQLDataSource>();
	GraphQlDataSource->Initialize(GraphQLUrl);

	SessionRepository = NewObject<USessionRepository>();
	SessionRepository->Initialize(GraphQlDataSource);
	
	UserRepository = NewObject<UUserRepository>();
	UserRepository->Initialize(GraphQlDataSource);

	PerformanceRepository = NewObject<UPerformanceRepository>();
	PerformanceRepository->Initialize(GraphQlDataSource);
	
	UsdSceneRepository = NewObject<UUsdSceneRepository>();
	UsdSceneRepository->Initialize(GraphQlDataSource);
}

void UPremiereCMSManagementSubsystem::TestGraphQlQueryFString() const
{
	FString Query = TEXT(R"(
		query sdafsdfasdfasdf ($sessionId: ID!, $userId: ID!) {
		  sessionById(id: $sessionId) {
			id
			title
		  }
		  users {
			id
			name
		  }
		  userById(id: $userId) {
			id
			eosId
		  }
		}
	)");

	const TMap<FString, FString> Variables = {
		{"sessionId", TEXT("1")},
		{"userId", TEXT("1")},
	};
	
	FOnGraphQLResponse OnGraphQlResponse;
	OnGraphQlResponse.BindLambda([](FGraphQLResult GraphQLResult)
	{
		UE_LOG(LogPremiereCMSManagementTest, Warning, TEXT("Test %s"), *GraphQLResult.RawResponse)
	});
	
	GraphQlDataSource->ExecuteGraphQLQuery(
		Query,
		Variables,
		OnGraphQlResponse
	);
}

void UPremiereCMSManagementSubsystem::TestGraphQlQueryFVariant() const
{
	FString Query = TEXT(R"(
		query sdafsdfasdfasdf ($sessionId: ID!, $userId: ID!) {
		  sessionById(id: $sessionId) {
			id
			title
		  }
		  users {
			id
			name
		  }
		  userById(id: $userId) {
			id
			eosId
		  }
		}
	)");

	const TMap<FString, FVariant> Variables = {
		{"sessionId", TEXT("1")},
		{"userId", 1},
	};
	
	FOnGraphQLResponse OnGraphQLResponse;
	OnGraphQLResponse.BindLambda([](FGraphQLResult GraphQLResult)
	{
		UE_LOG(LogPremiereCMSManagementTest, Warning, TEXT("Test %s"), *GraphQLResult.RawResponse)
	});
	
	GraphQlDataSource->ExecuteGraphQLQuery(
		Query,
		Variables,
		OnGraphQLResponse
	);
}

void UPremiereCMSManagementSubsystem::TestGraphQlQueryFJsonValue() const
{
	FString Query = TEXT(R"(
		query sdafsdfasdfasdf ($sessionId: ID!, $userId: ID!) {
		  sessionById(id: $sessionId) {
			id
			title
		  }
		  users {
			id
			name
		  }
		  userById(id: $userId) {
			id
			eosId
		  }
		}
	)");

	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"sessionId", MakeShared<FJsonValueString>(TEXT("1"))},
		{"userId", MakeShared<FJsonValueNumber>(1)},
	};
	
	FOnGraphQLResponse OnGraphQLResponse;
	OnGraphQLResponse.BindLambda([](FGraphQLResult GraphQLResult)
	{
		UE_LOG(LogPremiereCMSManagementTest, Warning, TEXT("Test %s"), *GraphQLResult.RawResponse)
	});
	
	GraphQlDataSource->ExecuteGraphQLQuery(
		Query,
		Variables,
		OnGraphQLResponse
	);
}

void UPremiereCMSManagementSubsystem::CreateSession(
	const FCMSSessionCreateInput& Data,
	const FString& OwnerWhereId,
	const EGQLSessionState SessionState,
	const TArray<FString>& AudioDataWhereIds,
	const TArray<FString>& FaceDataWhereIds,
	const FOnGetSession& OnCreateSessionSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	TArray<FCMSIdInput> FaceDataWhereIdStructs;
	for (const auto& Id : FaceDataWhereIds)
	{
		FaceDataWhereIdStructs.Add({Id});
	}
	
	TArray<FCMSIdInput> AudioDataWhereIdStructs;
	for (const auto& Id : AudioDataWhereIds)
	{
		AudioDataWhereIdStructs.Add({Id});
	}
	
	const FString SessionStateId = GetSessionStateId(SessionState);
	
	SessionRepository->CreateSession(
		Data,
		{ SessionStateId  },
		{ OwnerWhereId  },
		AudioDataWhereIdStructs,
		FaceDataWhereIdStructs,
		[OnCreateSessionSuccess](const FCMSSession& Session)
		{
			OnCreateSessionSuccess.ExecuteIfBound(Session);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::GetAllSessions(
	const FOnGetSessions& OnGetAllSessionsSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	SessionRepository->GetAll(
		[OnGetAllSessionsSuccess](const TArray<FCMSSession>& Sessions)
		{
			OnGetAllSessionsSuccess.ExecuteIfBound(Sessions);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindSession(
	const FCMSSessionWhereUniqueInput Where,
	FOnGetSessions OnGetSessionsSuccess,
	FOnFailureDelegate OnFailure
)
{
	SessionRepository->FindSession(
		Where,
		[OnGetSessionsSuccess] (const TArray<FCMSSession>& Sessions)
		{
			OnGetSessionsSuccess.ExecuteIfBound(Sessions);
		},
		[OnFailure] (const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::CreateUser(
	const FCMSUserCreateInput& Data,
	const FString& PersonId,
	const FOnGetUserSuccess& OnCreateUserSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	TOptional<FCMSIdInput> PersonIdStruct;
	if (!PersonId.TrimStartAndEnd().IsEmpty())
	{
		PersonIdStruct = { PersonId.TrimStartAndEnd() };
	}
	
	UserRepository->Create(
		Data,
		PersonIdStruct,
		[OnCreateUserSuccess](const FCMSUser& User)
		{
			OnCreateUserSuccess.ExecuteIfBound(User);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::GetAllPerformances(
	FOnGetAllPerformancesSuccess OnGetAllPerformancesSuccess,
	FOnFailureDelegate OnGetAllPerformanceFailure
) const
{
	PerformanceRepository->GetAllPerformances(
		[OnGetAllPerformancesSuccess](const TArray<FCMSPerformance>& Performances)
		{
			OnGetAllPerformancesSuccess.ExecuteIfBound(Performances);
		},
		[OnGetAllPerformanceFailure](const FString& ErrorReason)
		{
			OnGetAllPerformanceFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindPerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	FOnGetPerformanceSuccess OnGetPerformanceSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->FindPerformance(
		Where,
		[OnGetPerformanceSuccess](const FCMSPerformance& Performance)
		{
			OnGetPerformanceSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::AddUsdSceneToPerformance(
	const FCMSUsdScenePerformanceWhereInput& Where,
	FOnGetPerformanceSuccess OnUsdSceneAddSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->AddUsdScene(
		Where,
		[OnUsdSceneAddSuccess](const FCMSPerformance& Performance)
		{
			OnUsdSceneAddSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::RemoveUsdSceneFromPerformance(
	const FCMSUsdScenePerformanceWhereInput& Where,
	FOnGetPerformanceSuccess OnUsdSceneRemoveSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->RemoveUsdScene(
		Where,
		[OnUsdSceneRemoveSuccess](const FCMSPerformance& Performance)
		{
			OnUsdSceneRemoveSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::AddUserToPerformance(
	const FCMSUserPerformanceWhereUniqueInput& Where,
	FOnGetPerformanceSuccess OnUserAddSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->AddMember(
		Where,
		[OnUserAddSuccess](const FCMSPerformance& Performance)
		{
			OnUserAddSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::RemoveUserFromPerformance(
	const FCMSUserPerformanceWhereUniqueInput& Where,
	FOnGetPerformanceSuccess OnUserRemoveSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->RemoveMember(
		Where,
		[OnUserRemoveSuccess](const FCMSPerformance& Performance)
		{
			OnUserRemoveSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::DeletePerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	FOnGetPerformanceSuccess OnGetPerformanceSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->DeletePerformance(
		Where,
		[OnGetPerformanceSuccess](const FCMSPerformance& Performance)
		{
			OnGetPerformanceSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::UpdatePerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	const FCMSPerformanceUpdateInput& Data,
	FOnGetPerformanceSuccess OnGetPerformanceSuccess,
	FOnFailureDelegate OnFailure
)
{
	PerformanceRepository->UpdatePerformance(
		Where,
		Data,
		[OnGetPerformanceSuccess](const FCMSPerformance& Performance)
		{
			OnGetPerformanceSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}


void UPremiereCMSManagementSubsystem::GetAllUsers(
	FOnGetAllUsersSuccess OnGetAllUsersSuccess,
	FOnFailureDelegate OnFailure
)
{
	UserRepository->GetAll(
		[OnGetAllUsersSuccess](const TArray<FCMSUser>& Users)
		{
			OnGetAllUsersSuccess.ExecuteIfBound(Users);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindUser(
	const FCMSIdInput& Where,	
	const FOnGetUserSuccess& OnFindUserSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	UserRepository->Find(
		Where,
		[OnFindUserSuccess](const FCMSUser& User)
		{
			OnFindUserSuccess.ExecuteIfBound(User);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::CreatePerformance(
	const FCMSPerformanceCreateInput& CreatePerformanceInput,
	const FString& OwnerId,
	const FOnCreatePerformanceSuccess& OnCreatePerformanceSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	PerformanceRepository->CreatePerformance(
		CreatePerformanceInput,
		{OwnerId},
		[OnCreatePerformanceSuccess](const FCMSPerformance& Performance)
		{
			OnCreatePerformanceSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::GetActiveSessions(
	const FOnGetSessions& OnGetActiveSessions,
	const FOnFailureDelegate& OnFailure
)
{
	SessionRepository->GetActiveSessions(
		[OnGetActiveSessions](const TArray<FCMSSession>& Sessions)
		{
			UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("SessionRepository->GetActiveSessions"));
			OnGetActiveSessions.ExecuteIfBound(Sessions);
			UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("SessionRepository->GetActiveSessions %d"), Sessions.Num());
		},
		[OnFailure](const FString& ErrorReason)
		{
			UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("SessionRepository->GetActiveSessions Failed"));
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

/** UsdScenes **/
void UPremiereCMSManagementSubsystem::GetAllUsdScenes(
	FOnGetAllUsdScenesSuccess OnGetAllUsdScenesSuccess,
	FOnFailureDelegate OnFailure
)
{
	UsdSceneRepository->GetAll(
		[OnGetAllUsdScenesSuccess](const TArray<FCMSUsdScene>& UsdScenes)
		{
			OnGetAllUsdScenesSuccess.ExecuteIfBound(UsdScenes);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::CreateUsdScene(
	const FCMSUsdSceneCreateInput& UsdSceneCreateInput,
	const FString& OwnerId,
	FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
	FOnFailureDelegate OnFailure
)
{
	TOptional<FCMSIdInput> OwnerIdStruct;
	if (!OwnerId.TrimStartAndEnd().IsEmpty())
	{
		OwnerIdStruct = { OwnerId.TrimStartAndEnd() };
	}
	
	
	UsdSceneRepository->Create(
		UsdSceneCreateInput,
		OwnerIdStruct,
		[OnGetUsdSceneSuccess](const FCMSUsdScene& UsdScene)
		{
			OnGetUsdSceneSuccess.ExecuteIfBound(UsdScene);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::UpdateUsdScene(
	const FCMSUsdSceneWhereUniqueInput& Where,
	const FCMSUsdSceneUpdateInput& Data,
	FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
	FOnFailureDelegate OnFailure
)
{
	UsdSceneRepository->Update(
		Where,
		Data,
		[OnGetUsdSceneSuccess](const FCMSUsdScene& UsdScene)
		{
			OnGetUsdSceneSuccess.ExecuteIfBound(UsdScene);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindUsdScene(
	const FCMSUsdSceneWhereUniqueInput& Where,
	FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
	FOnFailureDelegate OnFailure
)
{
	UsdSceneRepository->Find(
		Where,
		[OnGetUsdSceneSuccess](const FCMSUsdScene& UsdScene)
		{
			OnGetUsdSceneSuccess.ExecuteIfBound(UsdScene);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::DeleteUsdScene(
	const FCMSUsdSceneWhereUniqueInput& Where,
	FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
	FOnFailureDelegate OnFailure
)
{
	UsdSceneRepository->Delete(
		Where,
		[OnGetUsdSceneSuccess](const FCMSUsdScene& UsdScene)
		{
			OnGetUsdSceneSuccess.ExecuteIfBound(UsdScene);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}
