
#include "GraphQLDataSource.h"

#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "LogPremiereCMSManagement.h"

void UGraphQLDataSource::Initialize(FString EndpointUrl)
{
    this->Endpoint = EndpointUrl;
}

void UGraphQLDataSource::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, FString>& Variables,
    const FOnGraphQLResponse OnComplete
)
{
    if (Endpoint.IsEmpty())
    {
        OnComplete.ExecuteIfBound(false, TEXT("No endpoint configured."));
        return;
    }

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
                // Handle unsupported type
                OnComplete.ExecuteIfBound(false, FString::Printf(TEXT("Unsupported variable type for key: %s"), *Pair.Key));
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
    // Check for general errors
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("UGraphQLDataSource: Request failed (no valid response)."));
        OnComplete.ExecuteIfBound(false, TEXT("Request failed or response is invalid."));
        return;
    }

    // Check HTTP status code
    int32 StatusCode = Response->GetResponseCode();
    if (StatusCode < 200 || StatusCode >= 300)
    {
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("UGraphQLDataSource: Request failed with status code %d"), StatusCode);
        OnComplete.ExecuteIfBound(false, Response->GetContentAsString());
        return;
    }

    // If we got here, we have a successful response (e.g. 200 OK)
    const FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("UGraphQLDataSourceImpl: Response: %s"), *ResponseString);

    // Invoke our callback with the data
    OnComplete.ExecuteIfBound(true, ResponseString);
}
