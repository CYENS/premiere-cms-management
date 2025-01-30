
#include "GraphQLDataSource.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "LogPremiereCMSManagement.h"
#include "Json.h"
#include "JsonUtilities.h"

void UGraphQLDataSource::Initialize(FString EndpointUrl)
{
    this->Endpoint = EndpointUrl;
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
    FOnGraphQLResponse OnComplete
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
    FOnGraphQLResponse OnComplete
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

    const TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
    BodyObject->SetStringField(TEXT("query"), Query);
    
    if (Variables.Num() > 0)
    {
        const TSharedPtr<FJsonObject> VariablesObject = MakeShareable(new FJsonObject());
        for (const TPair<FString, TSharedPtr<FJsonValue>>& Pair : Variables)
        {
            VariablesObject->SetField(Pair.Key, Pair.Value);
        }
        BodyObject->SetObjectField(TEXT("variables"), VariablesObject);
    }

    FString BodyString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
    FJsonSerializer::Serialize(BodyObject.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(BodyString);

    HttpRequest->OnProcessRequestComplete().BindUObject(
        this,
        &ThisClass::OnRequestComplete,
        OnComplete
    );
    HttpRequest->ProcessRequest();
}

void UGraphQLDataSource::OnRequestComplete(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful,
    FOnGraphQLResponse OnComplete
)
{
    FGraphQLResult GraphQlResult;
    
    // general request errors
    if (!bWasSuccessful || !Response.IsValid())
    {
        GraphQlResult.GraphQLOutcome = RequestFailed;
        GraphQlResult.ErrorMessage = TEXT("Request failed or response is invalid.");
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *GraphQlResult.ErrorMessage);
        OnComplete.ExecuteIfBound(GraphQlResult);
        return;
    }
    
    GraphQlResult.RawResponse = Response->GetContentAsString();

    // HTTP status code
    const int32 StatusCode = Response->GetResponseCode();
    GraphQlResult.HttpStatus = StatusCode;
    if (StatusCode < 200 || StatusCode >= 300)
    {
        GraphQlResult.GraphQLOutcome = HttpError;
        GraphQlResult.ErrorMessage = Response->GetContentAsString();
        UE_LOG(
            LogPremiereCMSManagement,
            Error,
            TEXT("UGraphQLDataSource: Request failed with status code %d. Reason: %s"),
            StatusCode,
            *GraphQlResult.ErrorMessage
        );
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
