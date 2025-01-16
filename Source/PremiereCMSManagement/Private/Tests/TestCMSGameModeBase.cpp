
#include "Tests/TestCMSGameModeBase.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "SessionRepository.h"

void ATestCMSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GraphQlDataSource = NewObject<UGraphQLDataSource>();
	GraphQlDataSource->Initialize("http://localhost:4000/");

	SessionRepository = NewObject<USessionRepository>();
	SessionRepository->Initialize(GraphQlDataSource);
	
	TestExecuteGraphQLQuery();
	TestGetSessionById();
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
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session ID: %s"), *Session.Id);
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session Title: %s"), *Session.Title);
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session StreamingUrl: %s"), *Session.StreamingUrl);
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session AudioData Id: %s"), *Session.AudioDataId);
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("Session AudioData FileUrl: %s"), *Session.AudioDataFileUrl);
	});
	FOnFailure OnFailure;
	OnFailure.BindLambda([](const FString& ErrorReason)
	{
		UE_LOG(LogPremiereCMSManagementTest, Error, TEXT("Failed to get Session By Id %s"), *ErrorReason);
	});
	SessionRepository->GetSessionById("1", OnSuccess, OnFailure);
}
