

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "GraphQLDataSource.generated.h"


class UPremiereCMSDeveloperSettings;

enum EGraphQLOutcome
{
	Success,
	RequestFailed,
	HttpError,
	GraphQLError,
	ParseError,
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

	UPROPERTY()
	const UPremiereCMSDeveloperSettings* DeveloperSettings;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GraphQL")
	FString Endpoint;

	void Initialize(FString EndpointUrl, const UPremiereCMSDeveloperSettings* PremiereCMSDeveloperSettings);
	
	virtual void ExecuteGraphQLQuery(
		const FString& Query,
		const FOnGraphQLResponse OnComplete
	);

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
	
	virtual void ExecuteGraphQLQuery(
		const FString& Query,
		const FString& Variables,
		const FOnGraphQLResponse OnComplete
	);
	
	virtual void ExecuteGraphQLQuery(
		const FString& Query,
		const TSharedPtr<FJsonObject> Variables,
		const FOnGraphQLResponse OnComplete
	);

	void Login();
	
	static bool GetDataObjectFromResponse(
		const FString& JsonResponse,
		const FString& QueryName,
		TSharedPtr<FJsonObject>& OutObject,
		FString& OutErrorReason
	);

	static bool GetDataArrayFromResponse(
		const FString& JsonResponse,
		const FString& QueryName,
		const TArray<TSharedPtr<FJsonValue>>*& OutArray,
		FString& OutErrorReason
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
	bool ParseGraphQLResponse(
		const FString& ResponseString,
		FGraphQLResult& GraphQLResult
	);
	
	FString AuthenicationCookie;
};

/**
 * @tparam T FCMSStruct type of object to create
 * @param JsonResponse  The JsonResponse with a "data" key with multiple 
 * @param QueryName The inner query name to retrieve the data
 * @param OutItems The FCMSStruct objects to be filled
 * @param ParseSingleItem  The parse function to parse a JsonObject to a
 * @param OutErrorReason if it fails the reason why it failed
 * @return  false if it fails
 */
template <typename T>
bool ParseArrayOfItemsFromResponse(
    const FString& JsonResponse,
    const FString& QueryName,
    TArray<T>& OutItems,
    TFunctionRef<bool(const TSharedPtr<FJsonObject>&, T&, FString&)> ParseSingleItem,
    FString& OutErrorReason
);

template <typename T>
bool ParseSingleItemFromResponse(
	const FString& JsonResponse,
	const FString& QueryName,
	T& OutItem,
	TFunctionRef<bool(const TSharedPtr<FJsonObject>&, T&, FString&)> ParseSingleItem,
	FString& OutErrorReason
);

UENUM(BlueprintType)
enum class EGraphQLOperationType: uint8 {
	UpdateAudioData,
	UpdateUsdScene
};

struct FGraphQLOperationNames {
	FString WhereUniqueInputName;
	FString QueryName;
};

FGraphQLOperationNames GetGraphQLOperationNames(EGraphQLOperationType OperationType);
