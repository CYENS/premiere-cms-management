#pragma once

#include "Repositories/BaseRepository.h"

#include "GraphQLDataSource.h"
#include "JsonObjectConverter.h"
#include "LogPremiereCMSManagement.h"

void UBaseRepository::Initialize(UGraphQLDataSource* InDataSource)
{
    DataSource = InDataSource;
}

template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const FString& QueryName,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
    const TMap<FString, TSharedPtr<FJsonValue>> EmptyVariables;
    ExecuteGraphQLQuery<T>(
        Query,
        EmptyVariables,
        QueryName,
        OnSuccess,
        OnFailure
    );
}

template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
    const FString& QueryName,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure) const
{
    ExecuteGraphQLQuery<T>(
        Query,
        Variables,
        QueryName,
        [](const FGraphQLResult& Result, const FString& QueryName, T& OutStruct, FString& OutError)
        {
            FText OutErrorText;
            FJsonObjectConverter::JsonObjectToUStruct<T>(
                Result.ResponseObject->GetObjectField(TEXT("data"))->GetObjectField(QueryName).ToSharedRef(),
                &OutStruct,
                0,
                0,
                false,
                &OutErrorText
            );
            OutError = OutErrorText.ToString();
            return true;
        },
        OnSuccess,
        OnFailure
    );
}

/** Handles Single Object Response */
template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
    const FString& QueryName,
    TFunction<bool(const FGraphQLResult&, const FString&, T&, FString&)> ParseFunction,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
    FOnGraphQLResponse OnResponse;
    OnResponse.BindLambda([OnSuccess, OnFailure, QueryName, ParseFunction](const FGraphQLResult GraphQLResult)
    {
        if (GraphQLResult.GraphQLOutcome != Success)
        {
            OnFailure(GraphQLResult.ErrorMessage);
            return;
        }

        FString ErrorReason;
        T ParsedData;
        if (ParseFunction(GraphQLResult, QueryName, ParsedData, ErrorReason))
        {
            OnSuccess(ParsedData);
        }
        else
        {
            UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response. Reason: %s"), *ErrorReason);
            OnFailure(ErrorReason);
        }
    });

    DataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
}


template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const FString& QueryName,
    TFunction<void(const TArray<T>&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
    const TMap<FString, TSharedPtr<FJsonValue>> EmptyVariables;
    ExecuteGraphQLQuery<T>(
        Query,
        EmptyVariables,
        QueryName,
        OnSuccess,
        OnFailure
    );
}

template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
    const FString& QueryName,
    TFunction<void(const TArray<T>&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
    ExecuteGraphQLQuery<T>(
        Query,
        Variables,
        QueryName,
        [](const FGraphQLResult& Result, const FString& QueryName, TArray<T>& OutStructs , FString& OutError)
        {
            const auto JsonArray = Result.ResponseObject->GetObjectField(TEXT("data"))->GetArrayField(QueryName);
            for (
                const TSharedPtr<FJsonValue>& JsonValue : JsonArray
            )
            {
                const TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
                T OutStruct;
                FText OutErrorText;
                FJsonObjectConverter::JsonObjectToUStruct<T>(
                    JsonObject.ToSharedRef(),
                    &OutStruct,
                    0,
                    0,
                    false,
                    &OutErrorText
                );
                OutError = OutErrorText.ToString();
                OutStructs.Add(OutStruct);
            }
            return true;
        },
        OnSuccess,
        OnFailure
    );
}

/** Handles Array Response */
template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
    const FString& QueryName,
    TFunction<bool(const FGraphQLResult&, const FString&, TArray<T>&, FString&)> ParseFunction,
    TFunction<void(const TArray<T>&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
    FOnGraphQLResponse OnResponse;
    OnResponse.BindLambda([OnSuccess, OnFailure, QueryName, ParseFunction](const FGraphQLResult GraphQLResult)
    {
        if (GraphQLResult.GraphQLOutcome != Success)
        {
            OnFailure(GraphQLResult.ErrorMessage);
            return;
        }

        FString ErrorReason;
        TArray<T> ParsedArray;
        if (ParseFunction(GraphQLResult, QueryName, ParsedArray, ErrorReason))
        {
            OnSuccess(ParsedArray);
        }
        else
        {
            UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response array. Reason: %s"), *ErrorReason);
            OnFailure(ErrorReason);
        }
    });

    DataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
}

template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const FString& Variables,
    const FString& QueryName,
    const TFunction<void(const TArray<T>&)>& OnSuccess,
    const TFunction<void(const FString&)>& OnFailure
) const
{
    ExecuteGraphQLQuery<T>(
        Query,
        Variables,
        QueryName,
        [](const FGraphQLResult& Result, const FString& QueryName, TArray<T>& OutStructs , FString& OutError)
        {
            const auto JsonArray = Result.ResponseObject->GetObjectField(TEXT("data"))->GetArrayField(QueryName);
            for (
                const TSharedPtr<FJsonValue>& JsonValue : JsonArray
            )
            {
                const TSharedPtr<FJsonObject> JsonObject = JsonValue->AsObject();
                T OutStruct;
                FText OutErrorText;
                FJsonObjectConverter::JsonObjectToUStruct<T>(
                    JsonObject.ToSharedRef(),
                    &OutStruct,
                    0,
                    0,
                    false,
                    &OutErrorText
                );
                OutError = OutErrorText.ToString();
                OutStructs.Add(OutStruct);
            }
            return true;
        },
        OnSuccess,
        OnFailure
    );
}

template <typename T>
void UBaseRepository::ExecuteGraphQLQuery(
    const FString& Query,
    const FString& Variables,
    const FString& QueryName,
    const TFunction<bool(const FGraphQLResult&, const FString&, TArray<T>&, FString&)> ParseFunction,
    const TFunction<void(const TArray<T>&)> OnSuccess,
    const TFunction<void(const FString&)> OnFailure
) const
{
    FOnGraphQLResponse OnResponse;
    OnResponse.BindLambda([OnSuccess, OnFailure, QueryName, ParseFunction](const FGraphQLResult& GraphQLResult)
    {
        if (GraphQLResult.GraphQLOutcome != Success)
        {
            OnFailure(GraphQLResult.ErrorMessage);
            return;
        }

        FString ErrorReason;
        TArray<T> ParsedArray;
        if (ParseFunction(GraphQLResult, QueryName, ParsedArray, ErrorReason))
        {
            OnSuccess(ParsedArray);
        }
        else
        {
            UE_LOG(LogPremiereCMSManagement, Error, TEXT("Could not parse response array. Reason: %s"), *ErrorReason);
            OnFailure(ErrorReason);
        }
    });

    DataSource->ExecuteGraphQLQuery(Query, Variables, OnResponse);
}

template <typename T>
TSharedPtr<FJsonValueObject> UBaseRepository::MakeWhereValue(const T& WhereStruct, const bool OmitEmptyFields)
{
    return MakeJsonValueObjectFromUStruct(WhereStruct, OmitEmptyFields);
}

template <typename T>
TSharedPtr<FJsonValueObject> UBaseRepository::MakeDataValue(const T& WhereStruct, const bool OmitEmptyFields)
{
    return MakeJsonValueObjectFromUStruct(WhereStruct);
}

static TSharedPtr<FJsonValue> CustomExportCallbackForId(FProperty* Property, const void* Value)
{
    // We only override for our "Id" property 
    // (or if you want to do it for multiple properties, check for them here).
    if (Property && Property->GetName() == TEXT("Id"))
    {
        if (FStrProperty* StrProp = CastField<FStrProperty>(Property))
        {
            // Return the property’s string *as-is*, skipping StandardizeCase entirely.
            const FString StringValue = StrProp->GetPropertyValue(Value);
            return MakeShared<FJsonValueString>(StringValue);
        }
    }

    // Return nullptr so that for *all other* properties, 
    // the default FJsonObjectConverter logic applies.
    return TSharedPtr<FJsonValue>();
}

void UBaseRepository::FixId(TSharedPtr<FJsonObject>& JsonObject)
{
    const FString PossiblyMangledKey = TEXT("iD"); // or "Id", or "ID", etc. 
    if (JsonObject->HasField(PossiblyMangledKey))
    {
        TSharedPtr<FJsonValue> ValueForId = JsonObject->TryGetField(PossiblyMangledKey);
        JsonObject->RemoveField(PossiblyMangledKey);
        JsonObject->SetField(TEXT("id"), ValueForId);
    }
}

template <typename T>
TSharedPtr<FJsonValueObject> UBaseRepository::MakeJsonValueObjectFromUStruct(const T& UStruct, const bool OmitEmptyFields)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();

    if (!FJsonObjectConverter::UStructToJsonObject(
            T::StaticStruct(),
            &UStruct,
            JsonObject.ToSharedRef(),
            0,
            0
        )
    )
    {
        return nullptr;
    }

    FixId(JsonObject);
    
    if (OmitEmptyFields)
    {
        RemoveEmptyStringsFromJson(JsonObject);
    }
    
    return MakeShared<FJsonValueObject>(JsonObject);
}

void UBaseRepository::RemoveEmptyStringsFromJson(TSharedPtr<FJsonObject>& JsonObject)
{
    if (!JsonObject.IsValid())
    {
        return;
    }

    TArray<FString> KeysToRemove;
    for (const auto& Pair : JsonObject->Values)
    {
        if (Pair.Value->Type == EJson::String && Pair.Value->AsString().TrimStartAndEnd().IsEmpty())
        {
            KeysToRemove.Add(Pair.Key);
        }
    }

    for (const FString& Key : KeysToRemove)
    {
        JsonObject->Values.Remove(Key);
    }
}
