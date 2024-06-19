// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FLowDriveSpaceMuterModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	static EAppReturnType::Type OnModalMessageDialog(EAppMsgCategory InMessageCategory, EAppMsgType::Type InMessage, const FText& InText, const FText& InTitle);
private:
	static bool OldGIsRunningUnattendedScript;
};
