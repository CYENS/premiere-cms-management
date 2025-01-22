

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "GraphQLDataSource.generated.h"

/**
 * Delegate for receiving GraphQL responses asynchronously.
 * 
 * @param bSuccess Indicates if the request was successful (HTTP 200 OK, etc.)
 * @param Response Raw response content (usually JSON).
 */
DECLARE_DELEGATE_TwoParams(FOnGraphQLResponse, bool /*bSuccess*/, const FString& /*Response*/);

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
};
