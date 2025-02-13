
#include "PremiereCMSManagementSubsystem.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
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
	FCMSSession Session,
	FOnCreateSessionSuccessDelegate OnCreateSessionSuccess,
	FOnFailureDelegate OnCreateSessionFailure
)
{
	FOnGetSessionSuccess OnSuccess;
	OnSuccess.BindLambda([OnCreateSessionSuccess](const FCMSSession& Session)
	{
		OnCreateSessionSuccess.ExecuteIfBound(Session);
	});
	FOnFailure OnFailure;
	OnFailure.BindLambda([OnCreateSessionFailure](const FString& ErrorReason)
	{
		OnCreateSessionFailure.ExecuteIfBound(ErrorReason);
	});
	SessionRepository->CreateSession(
		Session,
		OnSuccess,
		OnFailure
	);
}

void UPremiereCMSManagementSubsystem::CreateUser(
	const FCMSUser& User,
	FOnCreateUserSuccess OnCreateSessionSuccess,
	FOnFailureDelegate OnCreateSessionFailure
)
{
	FOnGetUserSuccess OnSuccess;
	OnSuccess.BindLambda([OnCreateSessionSuccess](const FCMSUser& Session)
	{
		OnCreateSessionSuccess.ExecuteIfBound(Session);
	});
	FOnFailure OnFailure;
	OnFailure.BindLambda([OnCreateSessionFailure](const FString& ErrorReason)
	{
		OnCreateSessionFailure.ExecuteIfBound(ErrorReason);
	});
	UserRepository->CreateUser(
		User,
		OnSuccess,
		OnFailure
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

void UPremiereCMSManagementSubsystem::CreatePerformance(
	FCMSPerformanceCreateInput CreatePerformanceInput,
	FOnCreatePerformanceSuccess OnCreatePerformanceSuccess,
	FOnFailureDelegate OnCreatePerformanceFailure
)
{
	PerformanceRepository->CreatePerformance(
		CreatePerformanceInput,
		[OnCreatePerformanceSuccess](const FCMSPerformance& Performance)
		{
			OnCreatePerformanceSuccess.ExecuteIfBound(Performance);
		},
		[OnCreatePerformanceFailure](const FString& ErrorReason)
		{
			OnCreatePerformanceFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::GetActiveSessions(
	FOnGetActiveSessionsDelegate OnGetActiveSessions,
	FOnFailureDelegate OnGetActiveSessionsFailure)
{
	FOnGetActiveSessionsSuccess OnSuccess;
	OnSuccess.BindLambda([OnGetActiveSessions](const TArray<FCMSSession> Sessions)
	{
		OnGetActiveSessions.ExecuteIfBound(Sessions);
	});
	
	FOnFailure OnFailure;
	OnFailure.BindLambda([OnGetActiveSessionsFailure](const FString& ErrorReason)
	{
		OnGetActiveSessionsFailure.ExecuteIfBound(ErrorReason);
	});
	SessionRepository->GetActiveSessions(OnSuccess, OnFailure);
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
	FOnGetUsdSceneSuccess OnGetUsdSceneSuccess,
	FOnFailureDelegate OnFailure
)
{
	UsdSceneRepository->Create(
		UsdSceneCreateInput,
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
