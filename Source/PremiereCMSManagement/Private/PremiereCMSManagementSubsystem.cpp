
#include "PremiereCMSManagementSubsystem.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "Repositories/SessionRepository.h"

UPremiereCMSManagementSubsystem::UPremiereCMSManagementSubsystem()
{
	const FString DefaultGraphQlUrl = TEXT("https://premierecmsdev.medidata.pt/api/graphql");
	GraphQLUrl = GConfig->GetStringOrDefault(TEXT("CMSSettings"), TEXT("GraphQLUrl"), DefaultGraphQlUrl, GEngineIni);
	
	GraphQlDataSource = NewObject<UGraphQLDataSource>();
	GraphQlDataSource->Initialize(GraphQLUrl);

	SessionRepository = NewObject<USessionRepository>();
	SessionRepository->Initialize(GraphQlDataSource);

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
	OnGraphQlResponse.BindLambda([](bool bWasSuccesful, FString Response)
	{
		UE_LOG(LogPremiereCMSManagementTest, Warning, TEXT("Test %s"), *Response)
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
	OnGraphQLResponse.BindLambda([](bool bWasSuccesful, FString Response)
	{
		UE_LOG(LogPremiereCMSManagementTest, Warning, TEXT("Test %s"), *Response)
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
	OnGraphQLResponse.BindLambda([](bool bWasSuccesful, FString Response)
	{
		UE_LOG(LogPremiereCMSManagementTest, Warning, TEXT("Test %s"), *Response)
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
