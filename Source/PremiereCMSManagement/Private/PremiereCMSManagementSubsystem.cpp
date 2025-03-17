
#include "PremiereCMSManagementSubsystem.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "PremiereCMSDeveloperSettings.h"
#include "Repositories/AvatarRepository.h"
#include "Repositories/FileRepository.h"
#include "Repositories/GraphQLConstants.h"
#include "Repositories/PerformanceRepository.h"
#include "Repositories/PersonRepository.h"
#include "Repositories/SessionCastRepository.h"
#include "Repositories/SessionRepository.h"
#include "Repositories/UsdSceneRepository.h"
#include "Repositories/UserRepository.h"
#include "Structs/CMSInputs.h"

void UPremiereCMSManagementSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	DeveloperSettings = GetMutableDefault<UPremiereCMSDeveloperSettings>();
	
	GraphQLUrl = DeveloperSettings->GraphQLUrl;
	const FString ApiUrl = DeveloperSettings->GraphQLUrl;
	
	GraphQlDataSource = NewObject<UGraphQLDataSource>();
	GraphQlDataSource->Initialize(GraphQLUrl, DeveloperSettings);
	GraphQlDataSource->Login();

	SessionRepository = NewObject<USessionRepository>();
	SessionRepository->Initialize(GraphQlDataSource);
	
	UserRepository = NewObject<UUserRepository>();
	UserRepository->Initialize(GraphQlDataSource);

	PerformanceRepository = NewObject<UPerformanceRepository>();
	PerformanceRepository->Initialize(GraphQlDataSource);
	
	UsdSceneRepository = NewObject<UUsdSceneRepository>();
	UsdSceneRepository->Initialize(GraphQlDataSource);
	
	AvatarRepository = NewObject<UAvatarRepository>();
	AvatarRepository->Initialize(GraphQlDataSource);
	
	PersonRepository = NewObject<UPersonRepository>();
	PersonRepository->Initialize(GraphQlDataSource);
	
	FileRepository = NewObject<UFileRepository>();
	FileRepository->Initialize(GraphQlDataSource, ApiUrl);
	
	SessionCastRepository = NewObject<USessionCastRepository>();
	SessionCastRepository->Initialize(GraphQlDataSource);
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

void UPremiereCMSManagementSubsystem::ExecuteGraphQL(
	const FString Query,
	const FString Variables,
	const FOnGraphQLResponseDelegate& OnResponse
)
{
	GraphQlDataSource->ExecuteGraphQLQuery(
		Query,
		Variables,
		FOnGraphQLResponse::CreateLambda([OnResponse](FGraphQLResult Result)
		{
			
			const FCMSGraphQLResult GraphQLResult  {
					Result.GraphQLOutcome,
					Result.RawResponse,
					Result.HttpStatus,
					Result.ErrorMessage,
					Result.GraphQLErrors,
				};
			OnResponse.ExecuteIfBound(GraphQLResult);
		})
	);
}

void UPremiereCMSManagementSubsystem::CreateSession(
	const FCMSSessionCreateInput& Data,
	const FString& OwnerWhereId,
	const FString& UsdSceneWhereId,
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
	
	TOptional<FCMSIdInput> OwnerWhere;
	if (!OwnerWhereId.TrimStartAndEnd().IsEmpty())
	{
		OwnerWhere =  {OwnerWhereId.TrimStartAndEnd()} ;
	}
	
	TOptional<FCMSIdInput> UsdSceneWhere;
	if (!UsdSceneWhereId.TrimStartAndEnd().IsEmpty())
	{
		UsdSceneWhere =  {UsdSceneWhereId.TrimStartAndEnd()} ;
	}
	
	SessionRepository->CreateSession(
		Data,
		{ SessionStateId  },
	    OwnerWhere,
	    UsdSceneWhere,
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

void UPremiereCMSManagementSubsystem::UpdateSession(
	const FCMSSessionWhereUniqueInput& Where,
	const FCMSSessionUpdateInput& Data,
	const FString& OwnerWhereId,
	const FString& UsdSceneWhereId,
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
	
	TOptional<FCMSUserWhereUniqueInput> OwnerWhere;
	if (!OwnerWhereId.TrimStartAndEnd().IsEmpty())
	{
		OwnerWhere =  {OwnerWhereId.TrimStartAndEnd()} ;
	}
	
	TOptional<FCMSUsdSceneWhereUniqueInput> UsdSceneWhere;
	if (!UsdSceneWhereId.TrimStartAndEnd().IsEmpty())
	{
		UsdSceneWhere =  {UsdSceneWhereId.TrimStartAndEnd()} ;
	}
	
	SessionRepository->UpdateSession(
		Where,
		Data,
		{ SessionStateId  },
	    OwnerWhere,
	    UsdSceneWhere,
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

void UPremiereCMSManagementSubsystem::UpdateSessionVisibilityByEosSessionId(
	const FString& WhereEosSessionId,
	const ESessionVisibility NewSessionVisibility,
	const FOnGetSession& OnSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	SessionRepository->FindByEosSessionId(
		WhereEosSessionId,
		[&, WhereEosSessionId, NewSessionVisibility, OnSuccess, OnFailure](TArray<FCMSSession> Sessions)
	{
			if (Sessions.IsEmpty())
			{
				const FString ErrorReason = FString::Printf(TEXT("No session found with EosSessionId \"%s\""), *WhereEosSessionId);
				OnFailure.ExecuteIfBound(ErrorReason);
				return;
			}

			const FCMSSession FirstSession = Sessions[0];
			const EGQLSessionState CurrentSessionState = GetSessionStateFromSessionStateId(FirstSession.State.Id);
			EGQLSessionState NewSessionState;
			
			const bool IsSessionPublic = CurrentSessionState == EGQLSessionState::PublicInactive || CurrentSessionState == EGQLSessionState::PublicActive;
			const bool IsPreviousVisibilitySameAsNewVisibility = (IsSessionPublic && NewSessionVisibility == Public) || (!IsSessionPublic && NewSessionVisibility == Private);
			if (IsPreviousVisibilitySameAsNewVisibility)
			{
				// do nothing, just return the retrieved session
				OnSuccess.ExecuteIfBound(FirstSession);
				return;
			}

			switch (CurrentSessionState)
			{
				case EGQLSessionState::PublicInactive:
					NewSessionState = EGQLSessionState::PrivateInactive;
					break;
				case EGQLSessionState::PublicActive:
					NewSessionState = EGQLSessionState::PrivateActive;
					break;
				case EGQLSessionState::PrivateInactive:
					NewSessionState = EGQLSessionState::PublicInactive;
					break;
				case EGQLSessionState::PrivateActive:
					NewSessionState = EGQLSessionState::PublicActive;
					break;
			}

			FString NewSessionStateId = GetSessionStateId(NewSessionState);

			SessionRepository->UpdateSession(
				{FirstSession.Id },
				{},
				{ NewSessionStateId },
				{},
				{},
				{},
				{},
				[OnSuccess](const FCMSSession& Session)
				{
					OnSuccess.ExecuteIfBound(Session);
				},
				[OnFailure](const FString& ErrorReason)
				{
					OnFailure.ExecuteIfBound(ErrorReason);
				}
			);
	}, [OnFailure](const FString& ErrorReason)
	{
		OnFailure.ExecuteIfBound(ErrorReason);
	});
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
	const FCMSSessionWhereUniqueInput& Where,
	const FOnGetSession& OnGetSessionSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	SessionRepository->FindSession(
		Where,
		[OnGetSessionSuccess] (const FCMSSession& Session)
		{
			OnGetSessionSuccess.ExecuteIfBound(Session);
		},
		[OnFailure] (const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::UpdateSessionStateByEosSessionId(
	const FString& WhereEosSessionId,
	const EGQLSessionState SessionState,
	const FOnGetSession& OnSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	SessionRepository->UpdateSessionStateByEosSessionId(
		WhereEosSessionId,
		SessionState,
		[OnSuccess](const FCMSSession& Session)
		{
			OnSuccess.ExecuteIfBound(Session);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindSessionByEosSessionId(
	const FString& WhereEosSessionId,
	const FOnGetSession& OnFindSessionSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	SessionRepository->FindByEosSessionId(
		WhereEosSessionId,
		[OnFindSessionSuccess] (const TArray<FCMSSession>& Sessions)
		{
			if (Sessions.Num() > 0)
			{
				OnFindSessionSuccess.ExecuteIfBound(Sessions[0]);
			}
			else
			{
				const FCMSSession EmptyCMSSession {}; 
				OnFindSessionSuccess.ExecuteIfBound(EmptyCMSSession);
			}
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

void UPremiereCMSManagementSubsystem::UpdateUser(
	const FCMSUserWhereUniqueInput& WhereUser,
	const FCMSUserUpdateInput& Data,
	const FString& PersonId,
	const FOnGetUserSuccess& OnCreateUserSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	TOptional<FCMSPersonWhereUniqueInput> PersonIdStruct;
	if (!PersonId.TrimStartAndEnd().IsEmpty())
	{
		PersonIdStruct = { PersonId.TrimStartAndEnd() };
	}
	UserRepository->Update(
		WhereUser,
		Data,
		PersonIdStruct,
		[OnCreateUserSuccess](const FCMSUser& User)
		{
			OnCreateUserSuccess.ExecuteIfBound(User);
		},
		[OnFailure](const FString& ErrorReason) {
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::AddPersonToUser(
	const FCMSUserWhereUniqueInput& UserWhere,
	const FCMSPersonWhereUniqueInput& PersonWhere,
	const FOnGetUserSuccess& OnAddPersonToUserSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	UserRepository->ConnectPerson(
		UserWhere,
		PersonWhere,
		[OnAddPersonToUserSuccess](const FCMSUser& User)
		{
			OnAddPersonToUserSuccess.ExecuteIfBound(User);
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
	const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	const FCMSUsdSceneWhereUniqueInput& UsdSceneWhere,
	const FOnGetPerformanceSuccess& OnUsdSceneAddSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray UsdScenesToConnect { UsdSceneWhere };
	PerformanceRepository->ConnectUsdScenes(
		PerformanceWhere,
		UsdScenesToConnect,
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
	const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	const FCMSUsdSceneWhereUniqueInput& UsdSceneWhere,
	const FOnGetPerformanceSuccess& OnUsdSceneRemovedSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray UsdScenesToConnect { UsdSceneWhere };
	PerformanceRepository->DisconnectUsdScenes(
		PerformanceWhere,
		UsdScenesToConnect,
		[OnUsdSceneRemovedSuccess](const FCMSPerformance& Performance)
		{
			OnUsdSceneRemovedSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}


void UPremiereCMSManagementSubsystem::AddSessionToPerformance(
	const FCMSIdInput& PerformanceWhere,
	const FCMSIdInput& SessionWhere,
	const FOnGetPerformanceSuccess& OnUsdSceneAddSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray<FCMSIdInput> SessionsToConnect { { SessionWhere.Id } };
	PerformanceRepository->ConnectSessions(
		{ PerformanceWhere.Id },
		SessionsToConnect,
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

void UPremiereCMSManagementSubsystem::RemoveSessionFromPerformance(
	const FCMSIdInput& PerformanceWhere,
	const FCMSIdInput& SessionWhere,
	const FOnGetPerformanceSuccess& OnUsdSceneRemoveSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray<FCMSIdInput> SessionsToConnect { { SessionWhere.Id } };
	PerformanceRepository->DisconnectSessions(
		{ PerformanceWhere.Id },
		SessionsToConnect,
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
	const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	const FCMSUserWhereUniqueInput& UserWhere,
	const FOnGetPerformanceSuccess& OnUserAddSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray<FCMSUserWhereUniqueInput> MembersToConnect { { UserWhere.Id } };
	PerformanceRepository->ConnectMembers(
		PerformanceWhere,
		MembersToConnect,
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
	const FCMSPerformanceWhereUniqueInput& PerformanceWhere,
	const FCMSUserWhereUniqueInput& UserWhere,
	const FOnGetPerformanceSuccess& OnUserRemoveSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	TArray<FCMSUserWhereUniqueInput> MembersToConnect { {UserWhere } };
	PerformanceRepository->DisconnectMembers(
		PerformanceWhere,
		MembersToConnect,	
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

void UPremiereCMSManagementSubsystem::AddAvatarToPerformance(
	const FCMSIdInput& PerformanceWhere,
	const FCMSIdInput& AvatarWhere,
	const FOnGetPerformanceSuccess& OnAvatarAddSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray<FCMSIdInput> AvatarsToConnect { { AvatarWhere.Id } };
	PerformanceRepository->ConnectAvatars(
		{ PerformanceWhere.Id },
		AvatarsToConnect,
		[OnAvatarAddSuccess](const FCMSPerformance& Performance)
		{
			OnAvatarAddSuccess.ExecuteIfBound(Performance);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::RemoveAvatarFromPerformance(
	const FCMSIdInput& PerformanceWhere,
	const FCMSIdInput& AvatarWhere,
	const FOnGetPerformanceSuccess& OnAvatarRemoveSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	const TArray<FCMSIdInput> AvatarsToConnect { { AvatarWhere.Id } };
	PerformanceRepository->DisconnectAvatars(
		{ PerformanceWhere.Id },
		AvatarsToConnect,
		[OnAvatarRemoveSuccess](const FCMSPerformance& Performance)
		{
			OnAvatarRemoveSuccess.ExecuteIfBound(Performance);
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

void UPremiereCMSManagementSubsystem::CreateAvatar(
	const FCMSAvatarCreateInput& Data,
	const FString& PerformanceId,
	FOnGetAvatar OnCreateAvatarSuccess,
	FOnFailureDelegate OnFailure
)
{
	TOptional<FCMSPerformanceWhereUniqueInput> PerformanceWhereId;
	if (!PerformanceId.TrimStartAndEnd().IsEmpty())
	{
		PerformanceWhereId = { PerformanceId.TrimStartAndEnd() };
	}
	
	AvatarRepository->Create(
		Data,
		PerformanceWhereId,
		[OnCreateAvatarSuccess](const FCMSAvatar& Avatar)
		{
			OnCreateAvatarSuccess.ExecuteIfBound(Avatar);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::CreatePerson(
	const FCMSPersonCreateInput& Data,
	const FString& UserId,
	FOnGetPerson OnCreatePersonSuccess,
	FOnFailureDelegate OnFailure
)
{
	TOptional<FCMSUserWhereUniqueInput> UserWhereUniqueInput;
	if (!UserId.TrimStartAndEnd().IsEmpty())
	{
		UserWhereUniqueInput = { UserId.TrimStartAndEnd() };
	}
	PersonRepository->Create(
		Data,
		UserWhereUniqueInput,
		[OnCreatePersonSuccess](const FCMSPerson& Person)
		{
			OnCreatePersonSuccess.ExecuteIfBound(Person);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindPerson(
	const FCMSPersonWhereUniqueInput& Where,
	const FOnGetPerson& OnFindPersonSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	PersonRepository->Find(
		Where,
		[OnFindPersonSuccess](const FCMSPerson& Person)
		{
			OnFindPersonSuccess.ExecuteIfBound(Person);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::FindPersonByGivenNameAndFamilyName(
	const FString& GivenName,
	const FString& FamilyName,
	const FOnGetPerson& OnFindPersonSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	PersonRepository->WhereFamilyNameAndGivenName(
		GivenName,
		FamilyName,
		[OnFindPersonSuccess](const TArray<FCMSPerson>& People)
		{
			if (!People.IsEmpty())
			{
				OnFindPersonSuccess.ExecuteIfBound(People[0]);
			}
			else
			{
				const FCMSPerson EmptyPerson {};
				OnFindPersonSuccess.ExecuteIfBound(EmptyPerson);
			}
			
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::UploadFileToObject(
	const EGraphQLOperationType Operation,
	const FCMSIdInput& Where,
	const FString& FilePath,
	const FOnGetObjectWithFile& OnUploadSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	FileRepository->UploadToObject(
		Operation,
		Where,
		FilePath,
		[OnUploadSuccess](const FCMSObjectWithFile& ObjectWithFile)
		{
			OnUploadSuccess.ExecuteIfBound(ObjectWithFile);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}

void UPremiereCMSManagementSubsystem::CreateSessionCast(
	const FCMSAvatarWhereUniqueInput& AvatarWhere,
	const FCMSSessionWhereUniqueInput& SessionWhere,
	const FCMSUserWhereUniqueInput& UserWhere,
	const FOnGetSessionCast& OnSuccess,
	const FOnFailureDelegate& OnFailure
)
{
	SessionCastRepository->Create(
		AvatarWhere,
		SessionWhere,
		UserWhere,
		[OnSuccess](const FCMSSessionCast& Person)
		{
			OnSuccess.ExecuteIfBound(Person);
		},
		[OnFailure](const FString& ErrorReason)
		{
			OnFailure.ExecuteIfBound(ErrorReason);
		}
	);
}
