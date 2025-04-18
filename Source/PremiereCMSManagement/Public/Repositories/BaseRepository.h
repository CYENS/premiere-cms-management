#pragma once

#include "CoreMinimal.h"
#include "GraphQLDataSource.h"
#include "BaseRepository.generated.h"

struct FCMSPerformanceCreateInput;
class UGraphQLDataSource;

DECLARE_DELEGATE_OneParam(FOnFailure, FString);

template<class T> class TMyTemplate {};

UCLASS(Abstract)
class PREMIERECMSMANAGEMENT_API UBaseRepository : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "PremiereCMS")
    void Initialize(UGraphQLDataSource* InDataSource);

    template <typename TEnum>
    static FString EnumToString(TEnum EnumValue);
    
    static void RemoveEmptyStringsFromJson(TSharedPtr<FJsonObject>& JsonObject);

    static void FixId(TSharedPtr<FJsonObject>& JsonObject);

    template <typename T>
    void GetAll(
        const TFunction<void(const TArray<T>& Objects)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    template <typename T>
    void Find(
        const FString& WhereId,
	    const TFunction<void(const T& Object)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    template <typename T>
    void Delete(
        const FString& WhereId,
	    const TFunction<void(const T& Object)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;

    template <typename ObjectT, typename  CreateT>
    void Create(
        const CreateT& CreateData,
        const TFunction<void(const ObjectT& Object)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    template <typename ObjectT, typename  CreateT>
    void Update(
        const FString& WhereId,
        const CreateT& UpdateData,
        const TFunction<void(const ObjectT& Object)>& OnSuccess,
        const TFunction<void(const FString& ErrorReason)>& OnFailure
    ) const;
    
    template <typename T>
    void ConnectOneItemToObject(
        const FString& ObjectWhereId,
        const FString& ItemToConnectWhereId,
        const FString& ItemKeyName,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
    template <typename T>
    void DisconnectOneItemFromObject(
        const FString& ObjectWhereId,
        const FString& ItemKeyName,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
    template <typename T>
    void ConnectManyItemsToObject(
        const FString& ObjectWhereId,
        const TArray<FString>& ItemsToConnectWhereId,
        const FString& ItemKeyName,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
    template <typename T>
    void DisconnectManyItemsFromObject(
        const FString& ObjectWhereId,
        const TArray<FString>& ItemsWhereId,
        const FString& ItemKeyName,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
protected:
    virtual FString GetObjectType() const;
    virtual FString GetObjectQuerySelectionSet() const;
    virtual FString GetObjectFragments() const;
    
    virtual FString GetObjectWhereInputName() const;
    virtual FString GetObjectWhereUniqueInputName() const;
    virtual FString GetObjectCreateInputName() const;
    virtual FString GetObjectUpdateInputName() const;
    virtual FString GetAllGraphQLQueryName() const;
    virtual FString GetWhereGraphQLQueryName() const;
    virtual FString GetFindQueryName() const;
    virtual FString GetDeleteQueryName() const;
    virtual FString GetCreateQueryName() const;
    virtual FString GetUpdateQueryName() const;

    UPROPERTY()
    UGraphQLDataSource* DataSource;

    /* Handle single object and it's overloads */
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const FString& QueryName,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;

    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
        const FString& QueryName,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
        const FString& QueryName,
        TFunction<bool(const FGraphQLResult&, const FString&, T&, FString&)> ParseFunction,
        TFunction<void(const T&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;
    
    /* Handle array object and it's overloads */
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const FString& QueryName,
        TFunction<void(const TArray<T>&)> OnSuccess,
        TFunction<void(const FString&)> OnFailure
    ) const;

    // No ParseFunction
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const TMap<FString, TSharedPtr<FJsonValue>>& Variables,
        const FString& QueryName,
        TFunction<void(const TArray<T>&)> OnSuccess,
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
    
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const FString& Variables,
        const FString& QueryName,
        const TFunction<void(const TArray<T>&)>& OnSuccess,
        const TFunction<void(const FString&)>& OnFailure
    ) const;
    
    template <typename T>
    void ExecuteGraphQLQuery(
        const FString& Query,
        const FString& Variables,
        const FString& QueryName,
        const TFunction<bool(const FGraphQLResult&, const FString&, TArray<T>&, FString&)> ParseFunction,
        const TFunction<void(const TArray<T>&)> OnSuccess,
        const TFunction<void(const FString&)> OnFailure
    ) const;
    
    template <typename T>
    static TSharedPtr<FJsonValueObject> MakeWhereValue(const T& WhereStruct, const bool OmitEmptyFields=true);
    
    template <typename T>
    static TSharedPtr<FJsonValueObject> MakeDataValue(const T& WhereStruct, const bool OmitEmptyFields=true);
    
    template <typename T>
    static TSharedPtr<FJsonValueObject> MakeJsonValueObjectFromUStruct(const T& UStruct, const bool OmitEmptyFields = true);
};
