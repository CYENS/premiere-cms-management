
#include "PremiereCMSManagementSubsystem.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "Repositories/PerformanceRepository.h"
#include "Repositories/SessionRepository.h"
#include "Repositories/UserRepository.h"
#include "Structs/CMSInputs.h"

UPremiereCMSManagementSubsystem::UPremiereCMSManagementSubsystem()
{
	const FString DefaultGraphQlUrl = TEXT("https://premierecmsdev.medidata.pt/api/graphql");
	GraphQLUrl = GConfig->GetStringOrDefault(TEXT("CMSSettings"), TEXT("GraphQLUrl"), DefaultGraphQlUrl, GEngineIni);
	
	GraphQlDataSource = NewObject<UGraphQLDataSource>();
	GraphQlDataSource->Initialize(GraphQLUrl);

	SessionRepository = NewObject<USessionRepository>();
	SessionRepository->Initialize(GraphQlDataSource);
	
	UserRepository = NewObject<UUserRepository>();
	UserRepository->Initialize(GraphQlDataSource);

	PerformanceRepository = NewObject<UPerformanceRepository>();
	PerformanceRepository->Initialize(GraphQlDataSource);
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
	FOnGetPerformanceSuccessDel OnGetPerformanceSuccess,
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

void UPremiereCMSManagementSubsystem::DeletePerformance(
	const FCMSPerformanceWhereUniqueInput& Where,
	FOnGetPerformanceSuccessDel OnGetPerformanceSuccess,
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
	FOnGetPerformanceSuccessDel OnGetPerformanceSuccess,
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
	FOnFailureDelegate OnGetAllUsersFailure
)
{
	FOnGetUsersSuccess OnSuccess;
	OnSuccess.BindLambda([OnGetAllUsersSuccess](TArray<FCMSUser> Performance)
	{
		OnGetAllUsersSuccess.ExecuteIfBound(Performance);
	});
	FOnFailure OnFailure;
	OnFailure.BindLambda([OnGetAllUsersFailure](const FString& ErrorReason)
	{
		OnGetAllUsersFailure.ExecuteIfBound(ErrorReason);
	});
	UserRepository->GetAllUsers(
		OnSuccess,
		OnFailure
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
