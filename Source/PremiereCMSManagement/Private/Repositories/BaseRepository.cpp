#pragma once

#include "Repositories/BaseRepository.h"

#include "DataObjectBuilder.h"
#include "GraphQLDataSource.h"
#include "JsonObjectConverter.h"
#include "LogPremiereCMSManagement.h"
#include "Repositories/GraphQLConstants.h"
#include "Structs/CMSInputs.h"

void UBaseRepository::Initialize(UGraphQLDataSource* InDataSource)
{
    DataSource = InDataSource;
}


template <typename TEnum>
FString UBaseRepository::EnumToString(TEnum EnumValue)
{
    static_assert(TIsEnum<TEnum>::Value, "EnumToString can only be used with enums.");

    const UEnum* EnumPtr = StaticEnum<TEnum>();
    if (!EnumPtr)
    {
        return TEXT("");
    }

    FString EnumValueAsString = EnumPtr->GetNameStringByValue(static_cast<int64>(EnumValue));
    EnumValueAsString[0] = FChar::ToLower(EnumValueAsString[0]);
    return EnumValueAsString;
}

template <typename T>
void UBaseRepository::GetAll(
	const TFunction<void(const TArray<T>& Sessions)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString ObjectType = GetObjectType();
	const FString ObjectFragments = GetObjectFragments();
	const FString QueryName = GetAllGraphQLQueryName();
	const FString ObjectSelectionSet = GetObjectQuerySelectionSet();
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query GetAll {
      %s {
		%s
      }
	}
	)"),
	*ObjectFragments,
	*QueryName,
	*ObjectSelectionSet
	);
	ExecuteGraphQLQuery(
		Query,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename T>
void UBaseRepository::Find(
	const FString& WhereId,
	const TFunction<void(const T& Object)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = GetFindQueryName();
	const FString Query = FString::Printf(TEXT(R"(
	%s
    query Find ($where: %s!){
      %s (where: $where) {
		%s
      }
	}
	)"),
	*GetObjectFragments(),
	*GetObjectWhereUniqueInputName(),
	*QueryName,
	*GetObjectQuerySelectionSet()
	);
	
    const FCMSIdInput WhereIdStruct { WhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(WhereIdStruct)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
        QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename T>
void UBaseRepository::Delete(
	const FString& WhereId,
	const TFunction<void(const T& Object)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString QueryName = GetDeleteQueryName();
	const FString Query = FString::Printf(TEXT(R"(
	%s
    mutation Delete ($where: %s!){
      %s (where: $where) {
		%s
      }
	}
	)"),
	*GetObjectFragments(),
	*GetObjectWhereUniqueInputName(),
	*QueryName,
	*GetObjectQuerySelectionSet()
	);
	
    const FCMSIdInput WhereIdStruct { WhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(WhereIdStruct)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
        QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename ObjectT, typename CreateT>
void UBaseRepository::Create(
    const CreateT& CreateData,
    const TFunction<void(const ObjectT& Object)>& OnSuccess,
    const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString ObjectFragments = GetObjectFragments();
	const FString ObjectCreateInputName = GetObjectCreateInputName();
	const FString QueryName = GetCreateQueryName();
	const FString QuerySelectionSet = GetObjectQuerySelectionSet();
	
	const FString Query = FString::Printf(TEXT(R"(
	%s
    mutation Create ($data: %s!){
      %s (data: $data) {
		%s
      }
	}
	)"),
	*ObjectFragments,
	*ObjectCreateInputName,
	*QueryName,
	*QuerySelectionSet
	);
	
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"data", MakeDataValue(CreateData)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
        QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename ObjectT, typename UpdateT>
void UBaseRepository::Update(
	const FString& WhereId,
	const UpdateT& UpdateData,
	const TFunction<void(const ObjectT& Object)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	const FString ObjectFragments = GetObjectFragments();
	const FString ObjectWhereUniqueInputName = GetObjectWhereUniqueInputName();
	const FString ObjectUpdateInputName = GetObjectUpdateInputName();
	const FString QueryName = GetUpdateQueryName();
	const FString QuerySelectionSet = GetObjectQuerySelectionSet();
	
	const FString Query = FString::Printf(TEXT(R"(
	%s
    mutation Update ($where: %s!, $data: %s!){
      %s (where: $where, data: $data) {
		%s
      }
	}
	)"),
	*ObjectFragments,
	*ObjectWhereUniqueInputName,
	*ObjectUpdateInputName,
	*QueryName,
	*QuerySelectionSet
	);

	const FCMSIdInput WhereIdStruct { WhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables = {
		{"where", MakeWhereValue(WhereIdStruct)},
		{"data", MakeDataValue(UpdateData)}
	};
	
	ExecuteGraphQLQuery(
		Query,
		Variables,
        QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename T>
void UBaseRepository::ConnectOneItemToObject(
    const FString& ObjectWhereId,
    const FString& ItemToConnectWhereId,
    const FString& ItemKeyName,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
	const FString QueryName = GetUpdateQueryName();
	const FString Query = FString::Printf(
		TEXT(R"(
		%s
		mutation Update ($where: %s!, $data: %s!) {
			%s (where: $where, data: $data) {
				%s
			}
		}
		)"),
		*GetObjectFragments(),
		*GetObjectWhereUniqueInputName(),
		*GetObjectUpdateInputName(),
		*QueryName,
		*GetObjectQuerySelectionSet()
	);

	FDataObjectBuilder ObjectBuilder;
    const FCMSIdInput ItemToConnectWhereIdStruct { ItemToConnectWhereId };
	ObjectBuilder.AddConnect(ItemKeyName, ItemToConnectWhereIdStruct);

    const FCMSIdInput ObjectWhereIdStruct { ObjectWhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables{
	{"where", MakeWhereValue(ObjectWhereIdStruct)},
	{"data", ObjectBuilder.BuildAsJsonValue()}
	};

	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename T>
void UBaseRepository::DisconnectOneItemFromObject(
    const FString& ObjectWhereId,
    const FString& ItemKeyName,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
	const FString QueryName = GetUpdateQueryName();
	const FString Query = FString::Printf(
		TEXT(R"(
		%s
		mutation Update ($where: %s!, $data: %s!) {
			%s (where: $where, data: $data) {
				%s
			}
		}
		)"),
		*GetObjectFragments(),
		*GetObjectWhereUniqueInputName(),
		*GetObjectUpdateInputName(),
		*QueryName,
		*GetObjectQuerySelectionSet()
	);

	FDataObjectBuilder ObjectBuilder;
	ObjectBuilder.AddDisconnect(ItemKeyName);

    const FCMSIdInput ObjectWhereIdStruct { ObjectWhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables{
	{"where", MakeWhereValue(ObjectWhereIdStruct)},
	{"data", ObjectBuilder.BuildAsJsonValue()}
	};

	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename T>
void UBaseRepository::ConnectManyItemsToObject(
    const FString& ObjectWhereId,
    const TArray<FString>& ItemsToConnectWhereId,
    const FString& ItemKeyName,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
	const FString QueryName = GetUpdateQueryName();
	const FString Query = FString::Printf(
		TEXT(R"(
		%s
		mutation Update ($where: %s!, $data: %s!) {
			%s (where: $where, data: $data) {
				%s
			}
		}
		)"),
		*GetObjectFragments(),
		*GetObjectWhereUniqueInputName(),
		*GetObjectUpdateInputName(),
		*QueryName,
		*GetObjectQuerySelectionSet()
	);

	FDataObjectBuilder ObjectBuilder;
    TArray<FCMSIdInput> ItemIdsAsStructs;
    for (const FString& ItemId : ItemsToConnectWhereId)
    {
        const FCMSIdInput ItemIdStruct { ItemId };
        ItemIdsAsStructs.Add( ItemIdStruct);
    }
	ObjectBuilder.AddConnect(ItemKeyName, ItemIdsAsStructs);

    const FCMSIdInput ObjectWhereIdStruct { ObjectWhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables{
	{"where", MakeWhereValue(ObjectWhereIdStruct)},
	{"data", ObjectBuilder.BuildAsJsonValue()}
	};

	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

template <typename T>
void UBaseRepository::DisconnectManyItemsFromObject(
    const FString& ObjectWhereId,
    const TArray<FString>& ItemsWhereId,
    const FString& ItemKeyName,
    TFunction<void(const T&)> OnSuccess,
    TFunction<void(const FString&)> OnFailure
) const
{
	const FString QueryName = GetUpdateQueryName();
	const FString Query = FString::Printf(
		TEXT(R"(
		%s
		mutation Update ($where: %s!, $data: %s!) {
			%s (where: $where, data: $data) {
				%s
			}
		}
		)"),
		*GetObjectFragments(),
		*GetObjectWhereUniqueInputName(),
		*GetObjectUpdateInputName(),
		*QueryName,
		*GetObjectQuerySelectionSet()
	);

	FDataObjectBuilder ObjectBuilder;
    TArray<FCMSIdInput> ItemIdsAsStructs;
    for (const FString& ItemId : ItemsWhereId)
    {
        const FCMSIdInput ItemIdStruct { ItemId };
        ItemIdsAsStructs.Add( ItemIdStruct);
    }
	ObjectBuilder.AddDisconnect(ItemKeyName, ItemIdsAsStructs);

    const FCMSIdInput ObjectWhereIdStruct { ObjectWhereId };
	const TMap<FString, TSharedPtr<FJsonValue>> Variables{
	{"where", MakeWhereValue(ObjectWhereIdStruct)},
	{"data", ObjectBuilder.BuildAsJsonValue()}
	};

	ExecuteGraphQLQuery(
		Query,
		Variables,
		QueryName,
		OnSuccess,
		OnFailure
	);
}

FString UBaseRepository::GetObjectType() const
{
	// examples: Session, Person, AudioData
    return TEXT("");
}

FString UBaseRepository::GetObjectQuerySelectionSet() const
{
    return GetObjectSelectionSetFromType(GetObjectType());
}

FString UBaseRepository::GetObjectFragments() const
{
    return GetObjectFragmentsFromType(GetObjectType());
}

FString UBaseRepository::GetAllGraphQLQueryName() const
{
	// example: Session -> session -> sessions
	FString GetAllQueryName = GetObjectType();
	GetAllQueryName[0] = FChar::ToLower(GetAllQueryName[0]);
	GetAllQueryName.Append(TEXT("s"));
	return GetAllQueryName;
}

FString UBaseRepository::GetWhereGraphQLQueryName() const
{
	// same as GetAllGraphQLQueryName
	return GetAllGraphQLQueryName();
}

FString UBaseRepository::GetFindQueryName() const
{
	// example: Session -> session
	FString FindQueryName = GetObjectType();
	FindQueryName[0] = FChar::ToLower(FindQueryName[0]);
	return FindQueryName;
}

FString UBaseRepository::GetDeleteQueryName() const
{
	// example: deleteSession
	return FString::Printf(TEXT("delete%s"), *GetObjectType());
}

FString UBaseRepository::GetCreateQueryName() const
{
	// example: createSession
	return FString::Printf(TEXT("create%s"), *GetObjectType());
}

FString UBaseRepository::GetUpdateQueryName() const
{

	// example: updateSession
    return FString::Printf(TEXT("update%s"), *GetObjectType());
}

FString UBaseRepository::GetObjectWhereInputName() const
{
	// example: SessionWhereInput
    return FString::Printf(TEXT("%sWhereInput"), *GetObjectType());
}

FString UBaseRepository::GetObjectWhereUniqueInputName() const
{
	// example: SessionWhereUniqueInput
    return FString::Printf(TEXT("%sWhereUniqueInput"), *GetObjectType());
}

FString UBaseRepository::GetObjectCreateInputName() const
{
	// example: SessionCreateInput
    return FString::Printf(TEXT("%sCreateInput"), *GetObjectType());
}

FString UBaseRepository::GetObjectUpdateInputName() const
{
	// example: SessionUpdateInput
    return FString::Printf(TEXT("%sUpdateInput"), *GetObjectType());
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
