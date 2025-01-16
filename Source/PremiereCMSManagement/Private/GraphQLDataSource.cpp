
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
        OnComplete.ExecuteIfBound(false, TEXT("No endpoint configured."));
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
