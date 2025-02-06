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
    TFunction<void(const FString&)> OnFailure) const
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
)
const
{
    ExecuteGraphQLQuery<T>(
        Query,
        Variables,
        QueryName,
        [](const FGraphQLResult& Result, const FString& QueryName, TArray<T>& OutStructs , FString& OutError)
        {
            for (
                const auto JsonArray = Result.ResponseObject->GetObjectField(TEXT("data"))->GetArrayField(QueryName);
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
