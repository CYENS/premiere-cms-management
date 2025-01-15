// Copyright Epic Games, Inc. All Rights Reserved.

#include "PremiereCMSManagement.h"

#define LOCTEXT_NAMESPACE "FPremiereCMSManagementModule"

void FPremiereCMSManagementModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FPremiereCMSManagementModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPremiereCMSManagementModule, PremiereCMSManagement)