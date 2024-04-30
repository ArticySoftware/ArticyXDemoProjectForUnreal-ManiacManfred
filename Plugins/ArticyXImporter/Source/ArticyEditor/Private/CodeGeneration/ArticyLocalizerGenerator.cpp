//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyLocalizerGenerator.h"
#include "ArticyImportData.h"
#include "ArticyGlobalVariables.h"
#include "ArticyImporterHelpers.h"
#include "ArticyTypeSystem.h"
#include "CodeFileGenerator.h"
#include "CodeGenerator.h"
#include "Internationalization/StringTableRegistry.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Misc/ConfigCacheIni.h"

void ArticyLocalizerGenerator::GenerateCode(const UArticyImportData* Data, FString& OutFile)
{
	if(!ensure(Data))
		return;

	OutFile = CodeGenerator::GetArticyLocalizerClassname(Data, true);
	CodeFileGenerator(OutFile + ".h", true, [&](CodeFileGenerator* header)
	{
		header->Line("#include \"CoreUObject.h\"");
		header->Line("#include \"ArticyRuntime/Public/ArticyLocalizerSystem.h\"");
		header->Line("#include \"" + OutFile + ".generated.h\"");

		header->Line();

		// Generate the UArticyLocalizerSystem class
		const auto type = CodeGenerator::GetArticyLocalizerClassname(Data, false);
		header->Class(type + " : public UArticyLocalizerSystem", TEXT("Articy Localizer System"), true, [&]
		{
			header->AccessModifier("public");

			header->Method("void", "Reload", "", [&]
			{
				header->Line(TEXT("const FString& LangName = FInternationalization::Get().GetCurrentCulture()->GetTwoLetterISOLanguageName()"), true);
				header->Line(TEXT("if (!bListenerSet) {"));
				header->Line(FString::Printf(TEXT("FInternationalization::Get().OnCultureChanged().AddUObject(this, &%s::Reload)"), *type), true, true, 1);
				header->Line(TEXT("bListenerSet = true"), true, true, 1);
				header->Line(TEXT("}"));

				IterateStringTables(header, FPaths::ProjectContentDir() / "ArticyContent/Generated");

				FString L10NDir = FPaths::ProjectContentDir() / "L10N";
				if (FPaths::DirectoryExists(L10NDir))
				{
					for (const auto Language : Data->Languages.Languages)
					{
						FString LangPath = L10NDir / Language.Key / "ArticyContent/Generated";
						if (Language.Key.IsEmpty())
						{
							IterateStringTables(header, LangPath);
							continue;
						}
						header->Line(FString::Printf(TEXT("if (LangName.Equals(\"%s\")) {"), *Language.Key));
						IterateStringTables(header, LangPath, true);
						header->Line(TEXT("}"));
					}
				}
				header->Line(TEXT("bDataLoaded = true"), true);
			});
		});
	});

	// Path to the DefaultGame.ini
	FString IniFilePath = FPaths::ProjectConfigDir() + FString(TEXT("DefaultGame.ini"));

	// Section and Key to modify
	FString SectionName = FString(TEXT("/Script/UnrealEd.ProjectPackagingSettings"));
	FString KeyName = FString(TEXT("+DirectoriesToAlwaysCook"));
	FString NewValueToAdd = FString(TEXT("(Path=\"/Game/ArticyContent\")"));

	// Modify the INI file
	ModifyIniFile(IniFilePath, SectionName, KeyName, NewValueToAdd);
}

void ArticyLocalizerGenerator::IterateStringTables(CodeFileGenerator* Header, const FString& DirectoryPath, bool Indent)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	int IndentOffset = Indent ? 1 : 0;

	if (PlatformFile.DirectoryExists(*DirectoryPath))
	{
		TArray<FString> FoundFiles;
		PlatformFile.FindFiles(FoundFiles, *DirectoryPath, TEXT(".csv"));

		FString RelPath = DirectoryPath.Replace(*FPaths::ProjectContentDir(), TEXT(""), ESearchCase::IgnoreCase);

		for (const FString& FilePath : FoundFiles)
		{
			FString StringTable = FPaths::GetBaseFilename(*FilePath, true);
			Header->Line(FString::Printf(TEXT("FStringTableRegistry::Get().UnregisterStringTable(FName(\"%s\"))"), *StringTable), true, Indent, IndentOffset);
			Header->Line(FString::Printf(TEXT("LOCTABLE_FROMFILE_GAME(\"%s\", \"%s\", \"%s/%s.csv\")"), *StringTable, *StringTable, *RelPath, *StringTable), true, Indent, IndentOffset);
		}
	}
}

void ArticyLocalizerGenerator::ModifyIniFile(const FString& IniFilePath, const FString& SectionName, const FString& KeyName, const FString& NewValue)
{
	// Read the INI file
	FString OldValue;
	GConfig->GetString(*SectionName, *KeyName, OldValue, IniFilePath);

	// Check if the directory is already present
	if (!OldValue.Contains(NewValue))
	{
		// Modify the INI file
		FString CombinedValue = OldValue + NewValue;
		GConfig->SetString(*SectionName, *KeyName, *CombinedValue, IniFilePath);
		GConfig->Flush(false, IniFilePath);
	}
}
