
#include "Tests/TestCMSGameModeBase.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"
#include "Repositories/SessionRepository.h"

void ATestCMSGameModeBase::BeginPlay()
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
	
	Super::BeginPlay();
}

void ATestCMSGameModeBase::TestExecuteGraphQLQuery()
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
	OnResponse.BindLambda([](FGraphQLResult GraphQLResult)
	{
		const FString ResponseContent = GraphQLResult.RawResponse;
		if (GraphQLResult.GraphQLOutcome != Success)
		{
			UE_LOG(LogPremiereCMSManagementTest, Error, TEXT("GraphQL request failed! Response: %s"), *ResponseContent);
			return;
		}
		UE_LOG(LogPremiereCMSManagementTest, Log, TEXT("GraphQL request succeeded! Response: %s"), *ResponseContent);
	});
	GraphQlDataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
}


void ATestCMSGameModeBase::TestGetActiveSessions() 
{
	SessionRepository->GetActiveSessions(
		LogUStructs<FCMSSession>,
		LogError
	);
}

void ATestCMSGameModeBase::LogError(const FString& ErrorReason)
{
	UE_LOG(LogPremiereCMSManagementTest, Error, TEXT("Error: %s"), *ErrorReason);
}

template <typename U>
void ATestCMSGameModeBase::LogUStructs(const TArray<U>& Structs)
{
	for (const U& Struct : Structs)
	{
		LogUStruct(Struct);
	}
}

template <typename U>
void ATestCMSGameModeBase::LogUStruct(const U& Struct)
{
	const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(
		Struct,
		0,
		0
	);
	
	FString StructAsString;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&StructAsString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *StructAsString);
}
