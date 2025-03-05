#pragma once

#include "CoreMinimal.h"
#include "BaseRepository.h"
#include "Structs/CMSTypes.h"
#include "FileRepository.generated.h"

struct FCMSIdInput;
struct FCMSFile;

UCLASS()
class PREMIERECMSMANAGEMENT_API UFileRepository : public UBaseRepository
{
    GENERATED_BODY()

public:

	void Initialize(UGraphQLDataSource* InDataSource, const FString InApiUrl);
	
	void UploadToObject(
		const EGraphQLOperationType Operation,
		const FCMSIdInput& Where,
		const FString& FilePath,
		const TFunction<void(const FCMSObjectWithFile& File)>& OnSuccess,
		const TFunction<void(const FString& ErrorReason)>& OnFailure
	) const;
	
	static FString BuildOperationsString(
		const FCMSIdInput& WhereId,
		const FString& WhereUniqueInputName,
		const FString& QueryName
	);
	
    static bool ParseGraphQLResponseManual(
    	const FString& JsonString,
		const FString& QueryName,
    	FCMSObjectWithFile& OutObjectWithFile,
    	FString& OutErrorMessage
    );
	
    FString ApiUrl;
};
