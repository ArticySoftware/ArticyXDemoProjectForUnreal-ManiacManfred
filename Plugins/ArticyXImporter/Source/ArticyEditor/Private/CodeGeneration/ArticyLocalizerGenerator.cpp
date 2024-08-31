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

/**
 * @brief Generates the localizer code for the given Articy import data.
 *
 * This function creates a header file for the Articy localizer system and
 * modifies the project configuration to include necessary directories.
 *
 * @param Data The Articy import data to generate code for.
 * @param OutFile The output file name for the generated code.
 */
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

/**
 * @brief Iterates over string tables in a given directory and generates code for each table.
 *
 * This function registers and loads string tables from CSV files found in the specified directory path.
 *
 * @param Header The code file generator to write string table registration code.
 * @param DirectoryPath The path to the directory containing string table CSV files.
 * @param Indent Whether to indent the generated code lines.
 */
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
