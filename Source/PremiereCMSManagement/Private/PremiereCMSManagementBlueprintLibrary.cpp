
#include "PremiereCMSManagementBlueprintLibrary.h"

#include "Structs/CMSTypes.h"

EGQLSessionState UPremiereCMSManagementBlueprintLibrary::GetSessionStateFromId(const FString& SessionStateId)
{
	return GetSessionStateFromSessionStateId(SessionStateId);
}

EGQLSessionState UPremiereCMSManagementBlueprintLibrary::GetSessionStateFromStruct(const FCMSSessionState& SessionState)
{
	return GetSessionStateFromSessionStateId(SessionState.Id);
}

FString UPremiereCMSManagementBlueprintLibrary::GetSessionStateIdFromSessionState(const EGQLSessionState SessionState)
{
	return GetSessionStateId(SessionState);
}
