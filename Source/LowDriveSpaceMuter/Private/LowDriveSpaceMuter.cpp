// Copyright Epic Games, Inc. All Rights Reserved.

#include "LowDriveSpaceMuter.h"

#include "EditorSupportDelegates.h"

bool FLowDriveSpaceMuterModule::OldGIsRunningUnattendedScript = false;

void FLowDriveSpaceMuterModule::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("FLowDriveSpaceMuterModule"))

	//todo: need to find a better callback to be able to bind to ModalMessageDialog after  UEditorEngine::Init but before UUnrealEdEngine::ValidateFreeDiskSpace()
	// FCoreDelegates::ModalMessageDialog.BindStatic(&FLowDriveSpaceMuterModule::OnModalMessageDialog);
	
	FLowDriveSpaceMuterModule::OldGIsRunningUnattendedScript = GIsRunningUnattendedScript;
	
	FDelayedAutoRegisterHelper(EDelayedRegisterRunPhase::ObjectSystemReady,
		[]()
		{
			UE_LOG(LogTemp, Warning, TEXT("EndOfEngineInit => %s"), FCoreDelegates::ModalMessageDialog.IsBound() ? TEXT("BOUND !!"): TEXT(""))
			FLowDriveSpaceMuterModule::OldGIsRunningUnattendedScript = GIsRunningUnattendedScript;
			GIsRunningUnattendedScript = true;
		});
	
	FDelayedAutoRegisterHelper(EDelayedRegisterRunPhase::EndOfEngineInit,
		[]()
		{
			FCoreDelegates::ModalMessageDialog.BindUObject(GEditor, &UEditorEngine::OnModalMessageDialog);
			GIsRunningUnattendedScript = FLowDriveSpaceMuterModule::OldGIsRunningUnattendedScript;
		});
}

EAppReturnType::Type FLowDriveSpaceMuterModule::OnModalMessageDialog(EAppMsgCategory InMessageCategory, EAppMsgType::Type InMessage, const FText& InText, const FText& InTitle)
{
	const FText Title = NSLOCTEXT("DriveSpaceDialog", "LowHardDriveSpaceMsgTitle", "Warning: Low Drive Space");
	
	if (InTitle.EqualTo(Title))
	{
		UE_LOG(LogTemp, Warning, TEXT("Message Muted: '%s'\n%s"), *InTitle.ToString(), *InText.ToString())
	}
	else if (GEditor)
	{
		return GEditor->OnModalMessageDialog(InMessageCategory, InMessage, InText, InTitle);
	}
	return EAppReturnType::Ok;
}
	
IMPLEMENT_MODULE(FLowDriveSpaceMuterModule, LowDriveSpaceMuter)