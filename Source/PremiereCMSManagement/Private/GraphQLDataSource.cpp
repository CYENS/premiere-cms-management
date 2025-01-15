
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
    FOnGraphQLResponse OnComplete
)
{
    if (Endpoint.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("UGraphQLDataSourceImpl: Endpoint is not set!"));
        // Immediately invoke callback to signal failure.
        OnComplete.ExecuteIfBound(false, TEXT("No endpoint configured."));
        return;
    }

    // Create an HTTP request
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    // The default method for GraphQL calls is POST
    HttpRequest->SetURL(Endpoint);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // Build the JSON body with "query" and "variables"
    TSharedPtr<FJsonObject> BodyObject = MakeShareable(new FJsonObject());
    BodyObject->SetStringField(TEXT("query"), Query);

    // If we have any variables, set them as a nested JSON object
    if (Variables.Num() > 0)
    {
        TSharedPtr<FJsonObject> VariablesObject = MakeShareable(new FJsonObject());
        for (const TPair<FString, FString>& Pair : Variables)
        {
            // GraphQL variables can be of various types (ints, bools, etc.).
            // For simplicity, we’re just sending them as strings here.
            VariablesObject->SetStringField(Pair.Key, Pair.Value);
        }
        BodyObject->SetObjectField(TEXT("variables"), VariablesObject);
    }

    // Serialize the request body to JSON
    FString BodyString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&BodyString);
    FJsonSerializer::Serialize(BodyObject.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(BodyString);

    // Bind our completion handler, passing the callback so we can call it later
    HttpRequest->OnProcessRequestComplete().BindUObject(
        this,
        &ThisClass::OnRequestComplete,
        OnComplete
    );

    // Fire off the async request
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
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("UGraphQLDataSourceImpl: Request failed (no valid response)."));
        OnComplete.ExecuteIfBound(false, TEXT("Request failed or response is invalid."));
        return;
    }

    // Check HTTP status code
    int32 StatusCode = Response->GetResponseCode();
    if (StatusCode < 200 || StatusCode >= 300)
    {
        UE_LOG(LogPremiereCMSManagement, Error, TEXT("UGraphQLDataSourceImpl: Request failed with status code %d"), StatusCode);
        OnComplete.ExecuteIfBound(false, Response->GetContentAsString());
        return;
    }

    // If we got here, we have a successful response (e.g. 200 OK)
    const FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("UGraphQLDataSourceImpl: Response: %s"), *ResponseString);

    // Invoke our callback with the data
    OnComplete.ExecuteIfBound(true, ResponseString);
}
