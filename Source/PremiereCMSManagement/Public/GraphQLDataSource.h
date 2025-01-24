

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "GraphQLDataSource.generated.h"


enum EGraphQLOutcome
{
	Success,
	RequestFailed,
	HttpError,
	GraphQLError
};

USTRUCT()
struct FGraphQLResult
{
	GENERATED_BODY()
	
	EGraphQLOutcome GraphQLOutcome;

	FString ErrorMessage;
	
	int32 HttpStatus = 0;

	TArray<FString> GraphQLErrors;

	FString RawResponse;

	TSharedPtr<FJsonObject> ResponseObject;
};

/**
 * Delegate for receiving GraphQL responses asynchronously.
 * 
 * @param bSuccess Indicates if the request was successful (HTTP 200 OK, etc.)
 * @param Response Raw response content (usually JSON).
 */
// DECLARE_DELEGATE_TwoParams(FOnGraphQLResponse, bool /*bSuccess*/, const FString& /*Response*/);
DECLARE_DELEGATE_OneParam(FOnGraphQLResponse, FGraphQLResult /* GraphQLResult */ );

UCLASS()
class PREMIERECMSMANAGEMENT_API UGraphQLDataSource : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Configure the GraphQL endpoint (e.g., "https://api.example.com/graphql").
	 * In a real production environment, you might inject authentication tokens here as well.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GraphQL")
	FString Endpoint;

	void Initialize(FString EndpointUrl);

	virtual void ExecuteGraphQLQuery(
		const FString& Query,
		const TMap<FString, FString>& Variables,
		const FOnGraphQLResponse OnComplete
	);
	
	virtual void ExecuteGraphQLQuery(
		const FString& Query,
		const TMap<FString, FVariant>& Variables,
		const FOnGraphQLResponse OnComplete
	);
	
	virtual void ExecuteGraphQLQuery(
		const FString& Query,
		const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
		const FOnGraphQLResponse OnComplete
	);

private:
	/**
	 * Bound callback for HTTP request completion.
	 * Note: We pass the FOnGraphQLResponse so we can execute it when done.
	 */
	void OnRequestComplete(
		FHttpRequestPtr Request,
		FHttpResponsePtr Response,
		bool bWasSuccessful,
		FOnGraphQLResponse OnComplete
	);

	/**
	 * Parses the response string and fills the  GraphQLResult with the ResponseObject ( json object ) or with errors if
	 * we got an error from graphql.
	 *
	 * If the response string could be parsed returns true.
	 * 
	 * @param ResponseString The Response to parse
	 * @param GraphQLResult The result to fill 
	 * @return true if could parse the string and false otherwise
	 */
	static bool ParseGraphQLResponse(
		const FString& ResponseString,
		FGraphQLResult& GraphQLResult
	);
};
