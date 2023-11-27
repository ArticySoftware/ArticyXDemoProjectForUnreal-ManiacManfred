//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyRuntimeModule.h"
#include "Internationalization/StringTableRegistry.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

DEFINE_LOG_CATEGORY(LogArticyRuntime)

void FArticyRuntimeModule::StartupModule()
{
#if WITH_EDITOR
	FEditorDelegates::PostPIEStarted.AddRaw(this, &FArticyRuntimeModule::LoadStringTables);
#endif

	LoadStringTables(false);
	FInternationalization::Get().OnCultureChanged().AddRaw(this, &FArticyRuntimeModule::OnCultureChanged);
}

void FArticyRuntimeModule::ShutdownModule()
{
	FInternationalization::Get().OnCultureChanged().RemoveAll(this);
}

void FArticyRuntimeModule::OnCultureChanged()
{
	LoadStringTables(false);
}

void FArticyRuntimeModule::LoadStringTables(bool)
{
	const FString& LangName = FInternationalization::Get().GetCurrentCulture()->GetTwoLetterISOLanguageName();
	if (LangName.Len() > 0)
	{
		IterateStringTables(FPaths::ProjectContentDir() / "L10N" / LangName / "ArticyContent/Generated");
	}
	IterateStringTables(FPaths::ProjectContentDir() / "ArticyContent/Generated");
}

void FArticyRuntimeModule::IterateStringTables(const FString& DirectoryPath) const
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.DirectoryExists(*DirectoryPath))
	{
		TArray<FString> FoundFiles;
		PlatformFile.FindFiles(FoundFiles, *DirectoryPath, TEXT(".csv"));

		for (const FString& FilePath : FoundFiles)
		{
			FString StringTable = FPaths::GetBaseFilename(*FilePath, true);
			FStringTableRegistry::Get().UnregisterStringTable(FName(StringTable));
			FStringTableRegistry::Get().Internal_LocTableFromFile(
				FName(StringTable),
				StringTable,
				StringTable + ".csv",
				DirectoryPath);
		}
	}
}

IMPLEMENT_MODULE(FArticyRuntimeModule, ArticyRuntime)
