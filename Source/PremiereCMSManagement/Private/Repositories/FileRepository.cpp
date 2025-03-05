#include "Repositories/FileRepository.h"

#include "JsonObjectConverter.h"
#include "LogPremiereCMSManagement.h"
#include "Structs/CMSInputs.h"
#include "Structs/CMSTypes.h"

void UFileRepository::Initialize(UGraphQLDataSource* InDataSource, const FString InApiUrl)
{
	Super::Initialize(InDataSource);
	ApiUrl = InApiUrl;
}

void UFileRepository::UploadToObject(
	const EGraphQLOperationType Operation,
	const FCMSIdInput& Where,
	const FString& FilePath,
	const TFunction<void(const FCMSObjectWithFile& File)>& OnSuccess,
	const TFunction<void(const FString& ErrorReason)>& OnFailure
) const
{
	if (!FPaths::FileExists(FilePath))
	{
		const FString ErrorMessage = FString::Printf(TEXT("File does not exist: %s"), *FilePath);
		UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *ErrorMessage);
		OnFailure(ErrorMessage);
		return;
	}

	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *FilePath))
	{
		const FString ErrorMessage = FString::Printf(TEXT("Failed to load file: %s"), *FilePath);
		UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *ErrorMessage);
		OnFailure(ErrorMessage);
		return;
	}

	FString Boundary = FString::Printf(TEXT("---------------------------%s"), *FGuid::NewGuid().ToString());

	const auto [WhereUniqueInputName, QueryName] = GetGraphQLOperationNames(Operation);
	const FString Operations = BuildOperationsString(Where, WhereUniqueInputName, QueryName);
	const FString Map = TEXT("{\"0\": [\"variables.file\"]}");

	// Construct the multipart body as a string (the file binary will be appended later)
	FString MultipartBody;
	MultipartBody += TEXT("--") + Boundary + TEXT("\r\n");
	MultipartBody += TEXT("Content-Disposition: form-data; name=\"operations\"\r\n\r\n");
	MultipartBody += Operations + TEXT("\r\n");

	MultipartBody += TEXT("--") + Boundary + TEXT("\r\n");
	MultipartBody += TEXT("Content-Disposition: form-data; name=\"map\"\r\n\r\n");
	MultipartBody += Map + TEXT("\r\n");

	MultipartBody += TEXT("--") + Boundary + TEXT("\r\n");
	// Extract just the filename from the given file path
	FString FileName = FPaths::GetCleanFilename(FilePath);
	// Determine MIME type; for .mp4 we use video/mp4 (otherwise default to application/octet-stream)
	FString MimeType = TEXT("application/octet-stream");
	if (FPaths::GetExtension(FilePath).Equals(TEXT("mp4"), ESearchCase::IgnoreCase))
	{
		MimeType = TEXT("video/mp4");
	}
	MultipartBody += TEXT("Content-Disposition: form-data; name=\"0\"; filename=\"") + FileName + TEXT("\"\r\n");
	MultipartBody += TEXT("Content-Type: ") + MimeType + TEXT("\r\n\r\n");

	// Convert the initial part of the multipart body into binary data
	TArray<uint8> BodyData;
	{
		FTCHARToUTF8 Converter(*MultipartBody);
		BodyData.Append((uint8*)Converter.Get(), Converter.Length());
	}

	// Append the file binary data
	BodyData.Append(FileData);

	// Append the trailing boundary
	FString Trailing = TEXT("\r\n--") + Boundary + TEXT("--\r\n");
	{
		FTCHARToUTF8 Converter(*Trailing);
		BodyData.Append((uint8*)Converter.Get(), Converter.Length());
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetURL(ApiUrl);
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);
	HttpRequest->SetHeader(TEXT("x-apollo-operation-name"), TEXT("CreateVideoObjects"));
	DataSource->TryAddAuthenticationCookie(HttpRequest);
	HttpRequest->SetContent(BodyData);

	HttpRequest->OnProcessRequestComplete().BindLambda(
		[OnSuccess, OnFailure, QueryName](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (bWasSuccessful && Response.IsValid())
			{
				UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *Response->GetContentAsString());
				FCMSObjectWithFile Object;
				FString ErrorMessage;

				if (!ParseGraphQLResponseManual(Response->GetContentAsString(), QueryName, Object, ErrorMessage))
				{
					ErrorMessage = FString::Printf(TEXT("%s"), *ErrorMessage);
					UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *ErrorMessage);
					OnFailure(ErrorMessage);
					return;
				}
				OnSuccess(Object);
				UE_LOG(LogPremiereCMSManagement, Log, TEXT("File upload response: %s"),
				       *Response->GetContentAsString());
			}
			else
			{
				const FString ErrorMessage =
					Response.IsValid() ? Response->GetContentAsString() : TEXT("Unknown error");
				UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *ErrorMessage);
				OnFailure(ErrorMessage);
			}
		});

	if (!HttpRequest->ProcessRequest())
	{
		const FString ErrorMessage = TEXT("Failed to process the HTTP request.");
		UE_LOG(LogPremiereCMSManagement, Error, TEXT("%s"), *ErrorMessage);
		OnFailure(ErrorMessage);
	}
}

FString UFileRepository::BuildOperationsString(
	const FCMSIdInput& WhereId,
	const FString& WhereUniqueInputName,
	const FString& QueryName
)
{
	const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	const FString QueryString = FString::Printf(
		TEXT(R"(
			mutation CreateVideoObjects($where: %s!, $file: Upload!) {
				%s (where: $where, data: { file: { upload: $file } }) {
					id
					fileUrl
					file { filename filesize }
				}
			}
		)"),
		*WhereUniqueInputName,
		*QueryName
	);
	JsonObject->SetStringField(TEXT("query"), QueryString);

	// Build the "variables" object
	TSharedPtr<FJsonObject> VariablesObject = MakeShareable(new FJsonObject());

	// Build the "where" object with the provided ID
	TSharedPtr<FJsonObject> WhereObject = MakeShareable(new FJsonObject());
	WhereObject->SetStringField(TEXT("id"), WhereId.Id);
	VariablesObject->SetObjectField(TEXT("where"), WhereObject);

	// Set the "file" field to null
	VariablesObject->SetField(TEXT("file"), MakeShareable(new FJsonValueNull()));

	// Add the "variables" object to the main JSON object
	JsonObject->SetObjectField(TEXT("variables"), VariablesObject);

	// Serialize the JSON object into a string
	FString OperationsString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OperationsString);
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize JSON for operations"));
		return FString(); // Return an empty string on failure
	}

	return OperationsString;
}

bool UFileRepository::ParseGraphQLResponseManual(
	const FString& JsonString,
	const FString& QueryName,
	FCMSObjectWithFile& OutObjectWithFile,
	FString& OutErrorMessage
)
{
	TSharedPtr<FJsonObject> RootObject;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (
		!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid()
	)
	{
		OutErrorMessage = FString::Printf(TEXT("Could not deserialise response: %s"), *JsonString);
		return false;
	}

	const TSharedPtr<FJsonObject> DataObject = RootObject->GetObjectField(TEXT("data"));
	if (!DataObject.IsValid())
	{
		OutErrorMessage = TEXT("Failed to find 'data' object");
		return false;
	}

	if (
		TArray<TSharedPtr<FJsonValue>> Errors = RootObject->GetArrayField("errors");
		Errors.Num() > 0
	)
	{
		OutErrorMessage = Errors[0]->AsObject()->GetStringField(TEXT("message"));
		return false;
	}

	const TSharedPtr<FJsonObject> ObjectWithFileJson = DataObject->GetObjectField(QueryName);
	if (!ObjectWithFileJson.IsValid())
	{
		OutErrorMessage = FString::Printf(TEXT("Failed to find '%s' object"), *QueryName);
		return false;
	}

	if (
		FText ErrorText;
		!FJsonObjectConverter::JsonObjectToUStruct<FCMSObjectWithFile>(
			ObjectWithFileJson.ToSharedRef(),
			&OutObjectWithFile,
			0,
			0,
			false,
			&ErrorText
		))
	{
		OutErrorMessage = ErrorText.ToString();
		return false;
	}

	return true;
}
