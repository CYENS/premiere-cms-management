
#include "Tests/TestCMSGameModeBase.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "Repositories/SessionRepository.h"

void ATestCMSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GraphQlDataSource = NewObject<UGraphQLDataSource>();
	GraphQlDataSource->Initialize("http://localhost:4000/");

	SessionRepository = NewObject<USessionRepository>();
	SessionRepository->Initialize(GraphQlDataSource);
	
	// TestExecuteGraphQLQuery();
	TestGetSessionById();
	// TestGetActiveSessions();
}

void ATestCMSGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GraphQlDataSource = nullptr;
}

void ATestCMSGameModeBase::TestExecuteGraphQLQuery() const
{
	const FString Query = TEXT(R"(
	query {
	  users {
		id
		name
		email
		eosId
		performances {
		  id
		}
	  }
	}
	)");

	const TMap<FString, FString> Variables;
	
	FOnGraphQLResponse OnResponse;
	OnResponse.BindLambda([](bool bSuccess, const FString& ResponseContent)
	{
		if (!bSuccess)
		{
			UE_LOG(LogPremiereCMSManagementTest, Error, TEXT("GraphQL request failed! Response: %s"), *ResponseContent);
			return;
		}
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("GraphQL request succeeded! Response: %s"), *ResponseContent);
	});
	GraphQlDataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
}

void ATestCMSGameModeBase::TestGetSessionById() const
{
	FOnGetSessionSuccess OnSuccess;
	OnSuccess.BindLambda([](const FCMSSession& Session)
	{
		LogSession(Session);
	});
	FOnFailure OnFailure;
	OnFailure.BindLambda([](const FString& ErrorReason)
	{
		UE_LOG(LogPremiereCMSManagementTest, Error, TEXT("Failed to get Session By Id %s"), *ErrorReason);
	});
	SessionRepository->GetSessionById("1", OnSuccess, OnFailure);
	SessionRepository->GetSessionById("3", OnSuccess, OnFailure);
}

void ATestCMSGameModeBase::TestGetActiveSessions() const
{
	FOnGetActiveSessionsSuccess OnGetActiveSessions;
	OnGetActiveSessions.BindLambda([](const TArray<FCMSSession> Sessions)
	{
		for (auto Session : Sessions)
		{
			LogSession(Session);
		}
	});
	
	FOnFailure OnFailure;
	OnFailure.BindLambda([](const FString& ErrorReason)
	{
		UE_LOG(LogPremiereCMSManagementTest, Error, TEXT("Failed to get active sessions. Reason: %s"), *ErrorReason);
	});
	SessionRepository->GetActiveSessions(OnGetActiveSessions, OnFailure);
}
	
void ATestCMSGameModeBase::LogSession(const FCMSSession& Session)
{
	UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("-- Session --"));
	UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session ID: %s"), *Session.Id);
	UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session Title: %s"), *Session.Title);
	UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session StreamingUrl: %s"), *Session.StreamingUrl);
	UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session State: %s"), *Session.State);
	UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session Performance Id: %s"), *Session.PerformanceId);
	if (Session.AudioDataIds.Num() > 0)
	{
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("AudioData Ids:"));
		for (const FString& Id : Session.AudioDataIds)
		{
			UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("%s"), *Id);
		}
	}
}

