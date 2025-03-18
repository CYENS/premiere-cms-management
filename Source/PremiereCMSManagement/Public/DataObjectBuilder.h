#pragma once

#include "CoreMinimal.h"

/**
 * A fluent-style builder for creating JSON objects:
 *
 *   FDataObjectBuilder()
 *       .AddConnect("faceData", FaceDataIds)
 *       .AddConnect("performance", SinglePerformance)
 *       .AddUStruct(SomeStruct)
 *       .Build();
 *
 * Usage note:
 *   - T must be a USTRUCT with UPROPERTY fields for reflection when using AddConnect/AddUStruct.
 */
class FDataObjectBuilder
{
public:
    // Default constructor => creates an empty JSON object
    FDataObjectBuilder()
        : RootObject(MakeShareable(new FJsonObject()))
    {
    }

    /**
     * Add "Key": { "connect": [ ... ] } given an array of USTRUCTs
     *
     * Produces:
     *   "Key": {
     *     "connect": [
     *       {  // from struct #1  },
     *       {  // from struct #2  }
     *     ]
     *   }
     */
    template <typename T>
    FDataObjectBuilder& AddConnect(const FString& Key, const TArray<T>& Items);
    
    template <typename T>
    FDataObjectBuilder& AddDisconnect(const FString& Key, const TArray<T>& Items);

    /**
     * Add "Key": { "connect": { ... } } given a single USTRUCT
     *
     * Produces:
     *   "Key": {
     *     "connect": {
     *       // fields from the single struct 
     *     }
     *   }
     */
    template <typename T>
    FDataObjectBuilder& AddConnect(const FString& Key, const T& SingleItem);
    FDataObjectBuilder& AddDisconnect(const FString& Key);

    template <class T>
    FDataObjectBuilder& AddConnect(const FString& Key, const TOptional<T>& SingleItem);
    template <class T>
    FDataObjectBuilder& AddDisconnect(const FString& Key, const TOptional<T>& SingleItem);

    template <typename T>
    FDataObjectBuilder& AddCreate(const FString& Key, const T& Items);
    
    template <typename T>
    FDataObjectBuilder& AddCreate(const FString& Key, const TArray<T>& Items);
    
    /**
     * Merge *all fields* of a UStruct T directly into the *top-level* of the JSON object.
     *
     * If T has fields { foo = X, bar = Y } => the JSON object gets "foo": X, "bar": Y
     */
    template <typename T>
    FDataObjectBuilder& AddUStruct(const T& StructValue, const bool OmitEmptyFields = true);

    /**
     * Finish building and return the underlying JSON object.
     */
    TSharedPtr<FJsonObject> Build() const;
    
    TSharedPtr<FJsonValue> BuildAsJsonValue() const;

private:
    TSharedPtr<FJsonObject> RootObject;
};
