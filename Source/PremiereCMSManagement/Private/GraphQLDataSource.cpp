
#include "GraphQLDataSource.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "LogPremiereCMSManagement.h"
#include "PremiereCMSDeveloperSettings.h"

void UGraphQLDataSource::Initialize(FString EndpointUrl, const UPremiereCMSDeveloperSettings* PremiereCMSDeveloperSettings )
{
    this->Endpoint = EndpointUrl;
    this->DeveloperSettings = PremiereCMSDeveloperSettings;
}

void UGraphQLDataSource::ExecuteGraphQLQuery(const FString& Query, const FOnGraphQLResponse OnComplete)
{
    // empty variables for when query doesn't require them
    const TMap<FString, TSharedPtr<FJsonValue>> Variables;
    ExecuteGraphQLQuery(Query, Variables, OnComplete);
}

void UGraphQLDataSource::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, FString>& Variables,
    const FOnGraphQLResponse OnComplete
)
{
    TMap<FString, FVariant> VariantVariables;
    if (Variables.Num() > 0)
    {
        for (const TPair<FString, FString>& Pair : Variables)
        {
            FVariant VariantValue = Pair.Value;
            VariantVariables.Add(Pair.Key, VariantValue);
        }
        
    }

    ExecuteGraphQLQuery(Query, VariantVariables, OnComplete);
}

void UGraphQLDataSource::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, FVariant>& Variables,
    const FOnGraphQLResponse OnComplete
)
{
    TMap<FString, TSharedPtr<FJsonValue>> VariablesJson;
    if (Variables.Num() > 0)
    {
        const TSharedPtr<FJsonObject> VariablesObject = MakeShareable(new FJsonObject());
        for (const TPair<FString, FVariant>& Pair : Variables)
        {
            // Check the type of FVariant and set the appropriate JSON field
            if (Pair.Value.GetType() == EVariantTypes::String)
            {
                VariablesJson.Add(Pair.Key, MakeShared<FJsonValueString>(Pair.Value.GetValue<FString>()));
            }
            else if (Pair.Value.GetType() == EVariantTypes::Int32)
            {
                VariablesJson.Add(Pair.Key, MakeShared<FJsonValueNumber>(Pair.Value.GetValue<int32>()));
            }
            else if (Pair.Value.GetType() == EVariantTypes::Float)
            {
                VariablesJson.Add(Pair.Key, MakeShared<FJsonValueNumber>(Pair.Value.GetValue<float>()));
            }
            else if (Pair.Value.GetType() == EVariantTypes::Bool)
            {
                VariablesJson.Add(Pair.Key, MakeShared<FJsonValueBoolean>(Pair.Value.GetValue<bool>()));
            }
            // add more types here as we go
            else
            {
                FGraphQLResult GraphQLResult;
                GraphQLResult.GraphQLOutcome = RequestFailed;
                GraphQLResult.ErrorMessage = FString::Printf(TEXT("Unsupported variable type for key: %s"), *Pair.Key);
                OnComplete.ExecuteIfBound(GraphQLResult);
                return;
            }
        }
    }

    ExecuteGraphQLQuery(Query, VariablesJson, OnComplete);
}

void UGraphQLDataSource::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
    const FOnGraphQLResponse OnComplete
)
{
    const TSharedPtr<FJsonObject> VariablesObject = MakeShareable(new FJsonObject());
    if (Variables.Num() > 0)
    {
        for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : Variables)
        {
            VariablesObject->SetField(Pair.Key, Pair.Value);
        }
    }
    
    ExecuteGraphQLQuery(Query, VariablesObject,OnComplete);
}

void UGraphQLDataSource::ExecuteGraphQLQuery(
    const FString& Query,
    const FString& Variables,
    const FOnGraphQLResponse OnComplete
)
{
    TSharedPtr<FJsonObject> VariablesJsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Variables);
    if (Variables.TrimStartAndEnd().IsEmpty())
    {
        ExecuteGraphQLQuery(Query, OnComplete);
        return;
    }
    if (!FJsonSerializer::Deserialize(JsonReader, VariablesJsonObject) ||  !VariablesJsonObject.IsValid())
    {
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("Failed to deserialize variable: %s"), *Variables);
        FGraphQLResult GraphQLResult;
        GraphQLResult.GraphQLOutcome = ParseError;
        GraphQLResult.ErrorMessage = FString::Printf(TEXT("Failed to parse variable: %s"), *Variables);
        OnComplete.ExecuteIfBound(GraphQLResult);
        return;
    }
    
    ExecuteGraphQLQuery(Query, VariablesJsonObject, OnComplete);
}

void UGraphQLDataSource::ExecuteGraphQLQuery(
    const FString& Query,
    const TSharedPtr<FJsonObject> Variables,
    const FOnGraphQLResponse OnComplete
)
{
    if (Endpoint.IsEmpty())
    {
        FGraphQLResult Result;
        Result.GraphQLOutcome = EGraphQLOutcome::HttpError;
        Result.ErrorMessage = TEXT("No endpoint configured.");
        OnComplete.ExecuteIfBound(Result);
        return;
    }

    const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(Endpoint);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    TryAddAuthenticationCookie(HttpRequest);

    const TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
    BodyObject->SetStringField(TEXT("query"), Query);
    
    BodyObject->SetObjectField(TEXT("variables"), Variables);

    FString BodyString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
    FJsonSerializer::Serialize(BodyObject.ToSharedRef(), Writer);
    if (DeveloperSettings->ShouldLogQueries)
    {
        FString VariablesString;
        TSharedRef<TJsonWriter<>> VariablesWriter = TJsonWriterFactory<>::Create(&VariablesString);
        FJsonSerializer::Serialize(Variables.ToSharedRef(), VariablesWriter);
        
        UE_LOG(LogPremiereCMSManagementTest, Display, TEXT("Query: \n%s"), *Query);
        UE_LOG(LogPremiereCMSManagementTest, Display, TEXT("Variables: \n%s"), *VariablesString);
        if (DeveloperSettings->ShouldLogToEditor)
        {
            GEngine->AddOnScreenDebugMessage(-1, 7.5f, FColor::Cyan, FString::Printf(TEXT("Query:\n%s"), *Query));
            GEngine->AddOnScreenDebugMessage(-1, 7.5f, FColor::Cyan, FString::Printf(TEXT("Variables:\n%s"), *VariablesString));
        }
    }

    HttpRequest->SetContentAsString(BodyString);
    if (DeveloperSettings->ShouldLogHttpRequestHeaders) {
        const TArray<FString> AllHeaders = HttpRequest->GetAllHeaders();
        const FString HeaderString = FString::Join(AllHeaders, TEXT("\n"));
        UE_LOG(LogPremiereCMSManagement, Display, TEXT("HttpRequest Headers:\n%s"), *HeaderString);
    }

    HttpRequest->OnProcessRequestComplete().BindUObject(
        this,
        &ThisClass::OnRequestComplete,
        OnComplete
    );
    HttpRequest->ProcessRequest();
}

void UGraphQLDataSource::Login(const FOnGraphQLResponse& OnComplete)
{
	const FString AuthenticationQuery = TEXT(R"(
	mutation ($identity: String!, $secret: String!) {
	  authenticate: authenticateUserWithPassword(email: $identity, password: $secret) {
		... on UserAuthenticationWithPasswordSuccess {
		  item {
			id
			__typename
		  }
		  __typename
		}
		... on UserAuthenticationWithPasswordFailure {
		  message
		  __typename
		}
	 
		__typename
	  }
	}
	)");

	const FString AuthenticationVariables = FString::Printf(TEXT(R"(
	{
	  "identity": "%s",
	  "secret": "%s"
	}
	)"),
	*DeveloperSettings->AuthenticationIdentity,
	*DeveloperSettings->AuthenticationSecret
	);
	
	ExecuteGraphQLQuery(
		AuthenticationQuery,
		AuthenticationVariables,
		OnComplete
	);
}

void UGraphQLDataSource::OnRequestComplete(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful,
    FOnGraphQLResponse OnComplete
)
{
    FGraphQLResult GraphQlResult;

    const FString SetCookieHeader = Response->GetHeader(TEXT("Set-Cookie"));
    if (!SetCookieHeader.IsEmpty())
    {
        AuthenicationCookie = SetCookieHeader;
        UE_LOG(LogPremiereCMSManagement, Display, TEXT("Authentication Cookie: \n%s"), *AuthenicationCookie);
    }
    
    GraphQlResult.RawResponse = Response->GetContentAsString();
    if (DeveloperSettings->ShouldLogRawResponse)
    {
        UE_LOG(LogPremiereCMSManagement, Display, TEXT("Authentication Cookie: \n%s"), *AuthenicationCookie);
        UE_LOG(LogPremiereCMSManagement, Display, TEXT("Raw Response:\n%s"), *GraphQlResult.RawResponse);
        if (DeveloperSettings->ShouldLogToEditor)
        {
            GEngine->AddOnScreenDebugMessage(-1, 7.5f, FColor::Cyan, FString::Printf(TEXT("Raw Response:\n%s"), *GraphQlResult.RawResponse));
        }
    }

    // general request errors
    if (!bWasSuccessful || !Response.IsValid())
    {
        GraphQlResult.GraphQLOutcome = RequestFailed;
        GraphQlResult.ErrorMessage = TEXT("Request failed or response is invalid.");
        if (DeveloperSettings->ShouldLogErrors)
        {
            UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *GraphQlResult.ErrorMessage);
            if (DeveloperSettings->ShouldLogToEditor)
            {
                GEngine->AddOnScreenDebugMessage(-1, 12.5f, FColor::Red, FString::Printf(TEXT("Error Message:\n%s"), *GraphQlResult.ErrorMessage));
            }
        }
        OnComplete.ExecuteIfBound(GraphQlResult);
        return;
    }
    
    // HTTP status code
    const int32 StatusCode = Response->GetResponseCode();
    GraphQlResult.HttpStatus = StatusCode;
    if (StatusCode < 200 || StatusCode >= 300)
    {
        GraphQlResult.GraphQLOutcome = HttpError;
        GraphQlResult.ErrorMessage = Response->GetContentAsString();
        if (DeveloperSettings->ShouldLogErrors)
        {
            UE_LOG(LogPremiereCMSManagement, Error, TEXT("UGraphQLDataSource: Request failed with status code %d. Reason: %s"), StatusCode, *GraphQlResult.ErrorMessage );
            if (DeveloperSettings->ShouldLogToEditor)
            {
                GEngine->AddOnScreenDebugMessage(-1, 12.5f, FColor::Red, FString::Printf(TEXT("UGraphQLDataSource: Request failed with status code %d. Reason: %s"), StatusCode, *GraphQlResult.ErrorMessage));
            }
        }
        OnComplete.ExecuteIfBound(GraphQlResult);
        return;
    }

    const FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("UGraphQLDataSourceImpl: Response: %s"), *ResponseString);

    if (
        !ParseGraphQLResponse(
            ResponseString,
            GraphQlResult
        )
    )
    {
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("Failed to parse Response"));
    }

    OnComplete.ExecuteIfBound(GraphQlResult);
}

bool UGraphQLDataSource::ParseGraphQLResponse(
    const FString& ResponseString,
    FGraphQLResult& GraphQLResult
)
{
    if (
        const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
        
        FJsonSerializer::Deserialize(
            Reader,
            GraphQLResult.ResponseObject
        ) && GraphQLResult.ResponseObject.IsValid()
    )
    {
        // Check if we have an "errors" array
        if (GraphQLResult.ResponseObject->HasField("errors"))
        {
            GraphQLResult.GraphQLOutcome = GraphQLError;
            
            // Make sure there's at least one error object
            if (
                TArray<TSharedPtr<FJsonValue>> Errors = GraphQLResult.ResponseObject->GetArrayField("errors");
                Errors.Num() > 0
            )
            {
                for (const auto& ErrorJsonValue : Errors )
                {
                    const TSharedPtr<FJsonObject> ErrorJsonObject = ErrorJsonValue->AsObject();
                    const FString ErrorMessage = ErrorJsonObject->GetStringField("message");
                    GraphQLResult.GraphQLErrors.Add(ErrorMessage);
                    if (DeveloperSettings->ShouldLogErrors)
                    {
                        UE_LOG(LogPremiereCMSManagement, Error, TEXT("GraphQL Error: %s"), *ErrorMessage);
                        if (DeveloperSettings->ShouldLogToEditor)
                        {
                            GEngine->AddOnScreenDebugMessage(-1, 12.5f, FColor::Red, FString::Printf(TEXT("GraphQL Error: %s"), *ErrorMessage));
                        }
                        
                    }
                }
                GraphQLResult.ErrorMessage = GraphQLResult.GraphQLErrors[0]; // first error as the message
            }
        }
        else
        {
            GraphQLResult.GraphQLOutcome = Success;
        }
        return true;
    }
    return false;
}

FGraphQLOperationNames GetGraphQLOperationNames(EGraphQLOperationType OperationType)
{
    // Static mapping from enum values to operation names.
    static const TMap<EGraphQLOperationType, FGraphQLOperationNames> OperationMap = {
        { EGraphQLOperationType::UpdateAudioData, { TEXT("AudioDataWhereUniqueInput"), TEXT("updateAudioData") } },
        { EGraphQLOperationType::UpdateUsdScene, { TEXT("UsdSceneWhereUniqueInput"), TEXT("updateUsdScene") } }
    };

    if (const FGraphQLOperationNames* Found = OperationMap.Find(OperationType)) {
        return *Found;
    } else {
        UE_LOG(LogTemp, Error, TEXT("Invalid GraphQL operation type"));
        return { TEXT(""), TEXT("") };
    } 
}

bool UGraphQLDataSource::GetDataArrayFromResponse(
    const FString& JsonResponse,
    const FString& QueryName,
    const TArray<TSharedPtr<FJsonValue>>*& OutArray,
    FString& OutErrorReason
)
{
    const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);
    TSharedPtr<FJsonObject> RootObject;
    if (!FJsonSerializer::Deserialize(JsonReader, RootObject) || !RootObject.IsValid())
    {
        OutErrorReason = TEXT("could not deserialize json object");
        return false;
    }

    const TSharedPtr<FJsonObject> DataObject = RootObject->GetObjectField(TEXT("data"));
    if (!DataObject.IsValid())
    {
        OutErrorReason = TEXT("missing or invalid 'data' field in response");
        return false;
    }

    if (!DataObject->TryGetArrayField(QueryName, OutArray))
    {
        OutErrorReason = FString::Printf(
            TEXT("'%s' array is invalid or missing"),
            *QueryName
        );
        return false;
    }

    return true;
}

bool UGraphQLDataSource::TryAddAuthenticationCookie(const TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& HttpRequest)
{
    if (!AuthenicationCookie.IsEmpty())
    {
        HttpRequest->SetHeader(TEXT("Cookie"), AuthenicationCookie);
        return true;
    }
    return false;
}


bool UGraphQLDataSource::GetDataObjectFromResponse(
    const FString& JsonResponse,
    const FString& QueryName,
    TSharedPtr<FJsonObject>& OutObject,
    FString& OutErrorReason
)
{
    const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);
    TSharedPtr<FJsonObject> RootObject;
    if (!FJsonSerializer::Deserialize(JsonReader, RootObject) || !RootObject.IsValid())
    {
        OutErrorReason = TEXT("could not deserialize json object");
        return false;
    }

    const TSharedPtr<FJsonObject> DataObject = RootObject->GetObjectField(TEXT("data"));
    if (!DataObject.IsValid())
    {
        OutErrorReason = TEXT("missing or invalid 'data' field in response");
        return false;
    }

    OutObject = DataObject->GetObjectField(QueryName);
    if (!OutObject.IsValid())
    {
        OutErrorReason = FString::Printf(
            TEXT("'%s' object is invalid or missing"),
            *QueryName
        );
        return false;
    }

    return true;
}

template <typename T>
bool ParseArrayOfItemsFromResponse(
    const FString& JsonResponse,
    const FString& QueryName,
    TArray<T>& OutItems,
    TFunctionRef<bool(const TSharedPtr<FJsonObject>&, T&, FString&)> ParseSingleItem,
    FString& OutErrorReason
)
{
    const TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonResponse);
    TSharedPtr<FJsonObject> JsonObject;
    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
    {
        OutErrorReason = TEXT("Could not deserialize JSON object");
        return false;
    }

    // The GraphQL structure typically has a "data" field
    const TSharedPtr<FJsonObject> DataObject = JsonObject->GetObjectField(TEXT("data"));
    if (!DataObject.IsValid())
    {
        OutErrorReason = TEXT("\"data\" field is missing or invalid");
        return false;
    }

    const TArray<TSharedPtr<FJsonValue>>* ItemsArray;
    if (!DataObject->TryGetArrayField(QueryName, ItemsArray))
    {
        OutErrorReason = FString::Printf(TEXT("Array \"%s\" is invalid or missing"), *QueryName);
        return false;
    }

    for (const TSharedPtr<FJsonValue>& ItemValue : *ItemsArray)
    {
        const TSharedPtr<FJsonObject> ItemObject = ItemValue->AsObject();
        if (!ItemObject.IsValid())
        {
            OutErrorReason = TEXT("Invalid item object in array");
            return false;
        }

        T ParsedItem;
        if (!ParseSingleItem(ItemObject, ParsedItem, OutErrorReason))
        {
            // parseSingleItem sets OutErrorReason on failure
            return false;
        }

        OutItems.Add(ParsedItem);
    }

    return true;
}

template <typename T>
bool ParseSingleItemFromResponse(
    const FString& JsonResponse,
    const FString& QueryName,
    T& OutItem,
    TFunctionRef<bool(const TSharedPtr<FJsonObject>&, T&, FString&)> ParseSingleItem,
    FString& OutErrorReason
)
{
	TSharedPtr<FJsonObject> ItemObject;
	if (!UGraphQLDataSource::GetDataObjectFromResponse(JsonResponse, QueryName, ItemObject, OutErrorReason))
	{
		return false;
	}

	if (!ParseSingleItem(ItemObject, OutItem, OutErrorReason))
	{
		return false;
	}

	return true;
}
