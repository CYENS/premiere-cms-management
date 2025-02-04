#pragma once

#include "Repositories/BaseRepository.h"

#include "GraphQLDataSource.h"
#include "LogPremiereCMSManagement.h"

void UBaseRepository::Initialize(UGraphQLDataSource* InDataSource)
{
    DataSource = InDataSource;
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
