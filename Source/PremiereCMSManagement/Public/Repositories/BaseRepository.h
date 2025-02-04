#pragma once

#include "CoreMinimal.h"
#include "GraphQLDataSource.h"
#include "Structs/CMSTypes.h"
#include "BaseRepository.generated.h"

struct FCMSPerformanceCreateInput;
class UGraphQLDataSource;

DECLARE_DELEGATE_OneParam(FOnFailure, FString);

UCLASS(Abstract)
class PREMIERECMSMANAGEMENT_API UBaseRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "PremiereCMS")
    void Initialize(UGraphQLDataSource* InDataSource);

protected:
    UPROPERTY()
    UGraphQLDataSource* DataSource;

    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
        const FString& QueryName,
        TFunction<bool(const FGraphQLResult&, const FString&, T&, FString&)> ParseFunction,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
        const FString& QueryName,
        TFunction<bool(const FGraphQLResult&, const FString&, TArray<T>&, FString&)> ParseFunction,
        TFunction<void(const TArray<T>&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
};
