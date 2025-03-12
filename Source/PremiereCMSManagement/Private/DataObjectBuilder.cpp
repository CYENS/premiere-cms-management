#include "DataObjectBuilder.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "JsonObjectConverter.h"
#include "Repositories/BaseRepository.h"

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddConnect(const FString& Key, const TArray<T>& Items)
{
    {
        if (Items.Num() == 0)
        {
            // If desired, skip entirely when empty. 
            // Or create an empty connect array. Adjust as needed.
            return *this;
        }

        const TSharedPtr<FJsonObject> ConnectObject = MakeShareable(new FJsonObject());
        TArray<TSharedPtr<FJsonValue>> ConnectArray;
        for (const T& Item : Items)
        {
            TSharedPtr<FJsonObject> StructObj = MakeShareable(new FJsonObject());
            FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &Item, StructObj.ToSharedRef(), 0, 0);
            UBaseRepository::FixId(StructObj);
            ConnectArray.Add(MakeShareable(new FJsonValueObject(StructObj)));
        }
        ConnectObject->SetArrayField(TEXT("connect"), ConnectArray);

        RootObject->SetObjectField(Key, ConnectObject);
        return *this;
    }
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddDisconnect(const FString& Key, const TArray<T>& Items)
{
    {
        if (Items.Num() == 0)
        {
            // If desired, skip entirely when empty. 
            // Or create an empty connect array. Adjust as needed.
            return *this;
        }

        const TSharedPtr<FJsonObject> ConnectObject = MakeShareable(new FJsonObject());
        TArray<TSharedPtr<FJsonValue>> ConnectArray;
        for (const T& Item : Items)
        {
            TSharedPtr<FJsonObject> StructObj = MakeShareable(new FJsonObject());
            FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &Item, StructObj.ToSharedRef(), 0, 0);
            UBaseRepository::FixId(StructObj);
            ConnectArray.Add(MakeShareable(new FJsonValueObject(StructObj)));
        }
        ConnectObject->SetArrayField(TEXT("disconnect"), ConnectArray);

        RootObject->SetObjectField(Key, ConnectObject);
        return *this;
    }
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddConnect(const FString& Key, const T& SingleItem)
{
    // Convert single item
    TSharedPtr<FJsonObject> SingleObj = MakeShareable(new FJsonObject());
    FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &SingleItem, SingleObj.ToSharedRef(), 0, 0);
    UBaseRepository::FixId(SingleObj);

    const TSharedPtr<FJsonObject> ConnectObject = MakeShareable(new FJsonObject());
    ConnectObject->SetObjectField(TEXT("connect"), SingleObj);

    RootObject->SetObjectField(Key, ConnectObject);
    return *this;
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddDisconnect(const FString& Key, const T& SingleItem)
{
    TSharedPtr<FJsonObject> SingleObj = MakeShareable(new FJsonObject());
    FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &SingleItem, SingleObj.ToSharedRef(), 0, 0);
    UBaseRepository::FixId(SingleObj);

    const TSharedPtr<FJsonObject> ConnectObject = MakeShareable(new FJsonObject());
    ConnectObject->SetObjectField(TEXT("disconnect"), SingleObj);

    RootObject->SetObjectField(Key, ConnectObject);
    return *this;
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddConnect(const FString& Key, const TOptional<T>& SingleItem)
{
    if (SingleItem.IsSet())
    {
        AddConnect(Key, SingleItem.GetValue());
    }
    return *this;
}

template <class T>
FDataObjectBuilder& FDataObjectBuilder::AddDisconnect(const FString& Key, const TOptional<T>& SingleItem)
{
    if (SingleItem.IsSet())
    {
        AddDisconnect(Key, SingleItem.GetValue());
    }
    return *this;
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddCreate(const FString& Key, const TArray<T>& Items)
{
    {
        if (Items.Num() == 0)
        {
            // If desired, skip entirely when empty. 
            // Or create an empty connect array. Adjust as needed.
            return *this;
        }

        const TSharedPtr<FJsonObject> ConnectObject = MakeShareable(new FJsonObject());
        TArray<TSharedPtr<FJsonValue>> ConnectArray;
        for (const T& Item : Items)
        {
            TSharedPtr<FJsonObject> StructObj = MakeShareable(new FJsonObject());
            FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &Item, StructObj.ToSharedRef(), 0, 0);
            UBaseRepository::FixId(StructObj);
            ConnectArray.Add(MakeShareable(new FJsonValueObject(StructObj)));
        }
        ConnectObject->SetArrayField(TEXT("create"), ConnectArray);

        RootObject->SetObjectField(Key, ConnectObject);
        return *this;
    }
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddCreate(const FString& Key, const T& SingleItem)
{
    // Convert single item
    TSharedPtr<FJsonObject> SingleObj = MakeShareable(new FJsonObject());
    FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &SingleItem, SingleObj.ToSharedRef(), 0, 0);
    UBaseRepository::FixId(SingleObj);

    const TSharedPtr<FJsonObject> ConnectObject = MakeShareable(new FJsonObject());
    ConnectObject->SetObjectField(TEXT("connect"), SingleObj);

    RootObject->SetObjectField(Key, ConnectObject);
    return *this;
}

template <typename T>
FDataObjectBuilder& FDataObjectBuilder::AddUStruct(const T& StructValue, const bool OmitEmptyFields)
{
    // static_assert(TIsUStruct<T>::Value, "AddUStruct requires a USTRUCT type.");

    // Convert T to a temporary JSON object
    TSharedPtr<FJsonObject> TempObj = MakeShareable(new FJsonObject());
    FJsonObjectConverter::UStructToJsonObject(T::StaticStruct(), &StructValue, TempObj.ToSharedRef(), 0, 0);
    UBaseRepository::FixId(TempObj);
    
    if (OmitEmptyFields)
    {
        UBaseRepository::RemoveEmptyStringsFromJson(TempObj);
    }

    // Now merge the fields of TempObj into RootObject
    for (const auto& Pair : TempObj->Values)
    {
        RootObject->SetField(Pair.Key, Pair.Value);
    }

    return *this;
}

TSharedPtr<FJsonObject> FDataObjectBuilder::Build() const
{
    return RootObject;
}

TSharedPtr<FJsonValue> FDataObjectBuilder::BuildAsJsonValue() const
{
    return MakeShareable<FJsonValueObject>(new FJsonValueObject(RootObject));
}
