
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

void UPremiereCMSManagementSubsystem::CreateSession(
	const FString& Title,
	const FString& OwnerId,
	const FString& PerformanceId,
	const FString& State,
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
		Title,
		OwnerId,
		PerformanceId,
		State,
		OnSuccess, OnFailure
	);
}
