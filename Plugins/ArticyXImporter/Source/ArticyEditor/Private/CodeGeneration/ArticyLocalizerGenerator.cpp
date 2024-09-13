﻿//
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
    if (!ensure(Data))
        return;

    OutFile = CodeGenerator::GetArticyLocalizerClassname(Data, true);
    CodeFileGenerator(OutFile + ".h", true, [&](CodeFileGenerator* header)
        {
            header->Line("#include \"CoreUObject.h\"");
            header->Line("#include \"ArticyLocalizerSystem.h\"");
            header->Line("#include \"" + OutFile + ".generated.h\"");

            header->Line();

            // Generate the UArticyLocalizerSystem class
            const auto type = CodeGenerator::GetArticyLocalizerClassname(Data, false);
            header->Class(type + " : public UArticyLocalizerSystem", TEXT("Articy Localizer System"), true, [&]
                {
                    header->AccessModifier("public");

                    header->Method("void", "Reload", "", [&]
                        {
                            // Add listener for language/locale change events
                            header->Line(TEXT("if (!bListenerSet) {"));
                            header->Line(FString::Printf(TEXT("FInternationalization::Get().OnCultureChanged().AddUObject(this, &%s::Reload);"), *type), true, true, 1);
                            header->Line(TEXT("bListenerSet = true;"), true, true, 1);
                            header->Line(TEXT("}"));

                            // Declare variables for the current locale and language
                            header->Line(TEXT("FString LocaleName = FInternationalization::Get().GetCurrentCulture()->GetName();"));
                            header->Line(TEXT("FString LangName = FInternationalization::Get().GetCurrentCulture()->GetTwoLetterISOLanguageName();"));

                            // Fallback to default generated string tables if no localization is found
                            IterateStringTables(header, FPaths::ProjectContentDir() / "ArticyContent/Generated");

                            // Generate code to load the string tables for all available languages and locales.
                            IterateLocalizationDirectories(header, FPaths::ProjectContentDir() / "L10N");

                            header->Line(TEXT("bDataLoaded = true;"), true);
                        });
                });
        });

    // Path to the DefaultGame.ini
    FString IniFilePath = FPaths::ProjectConfigDir() + FString(TEXT("DefaultGame.ini"));
    FString SectionName = FString(TEXT("/Script/UnrealEd.ProjectPackagingSettings"));
    FString KeyName = FString(TEXT("+DirectoriesToAlwaysCook"));
    FString NewValueToAdd = FString(TEXT("(Path=\"/Game/ArticyContent\")"));

    // Modify the INI file
    ModifyIniFile(IniFilePath, SectionName, KeyName, NewValueToAdd);
}

void ArticyLocalizerGenerator::IterateLocalizationDirectories(CodeFileGenerator* Header, const FString& LocalizationRoot)
{
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

    // First, check if the root localization directory exists
    if (PlatformFile.DirectoryExists(*LocalizationRoot))
    {
        TArray<FString> LanguageDirs;
        PlatformFile.IterateDirectory(*LocalizationRoot, [&](const TCHAR* FilenameOrDirectory, bool bIsDirectory) {
            if (bIsDirectory)
            {
                // This will return the directory name (e.g., "en", "de", "pt-BR")
                FString LangCode = FPaths::GetCleanFilename(FilenameOrDirectory);

                // Now that we have the language code, we can create the path to its string tables
                FString LangPath = FString(FilenameOrDirectory) / "ArticyContent/Generated";

                // Generate code to load locale-specific files at runtime
                Header->Line(FString::Printf(TEXT("if (LocaleName == TEXT(\"%s\")) {"), *LangCode));
                IterateStringTables(Header, LangPath, true);
                Header->Line(TEXT("}"));

                // Handle fallback to general language if applicable
                FString GeneralLang = LangCode.Left(2);  // First two letters (e.g., "en" from "en-US")
                if (GeneralLang != LangCode) // Only generate if locale and language differ
                {
                    Header->Line(FString::Printf(TEXT("else if (LangName == TEXT(\"%s\")) {"), *GeneralLang));
                    IterateStringTables(Header, LangPath, true);
                    Header->Line(TEXT("}"));
                }
            }
            return true;  // Continue iterating
            });
    }
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
            Header->Line(FString::Printf(TEXT("FStringTableRegistry::Get().UnregisterStringTable(FName(\"%s\"));"), *StringTable), true, Indent, IndentOffset);
            Header->Line(FString::Printf(TEXT("LOCTABLE_FROMFILE_GAME(\"%s\", \"%s\", \"%s/%s.csv\");"), *StringTable, *StringTable, *RelPath, *StringTable), true, Indent, IndentOffset);
        }
    }
}

/**
 * @brief Modifies an INI file to add a new value to a specified section and key.
 *
 * This function reads the existing value for a given section and key, appends a new value if not already present,
 * and writes the updated value back to the INI file.
 *
 * @param IniFilePath The path to the INI file to modify.
 * @param SectionName The section in the INI file to modify.
 * @param KeyName The key within the section to modify.
 * @param NewValue The new value to add to the existing value.
 */
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
