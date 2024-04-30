//  
// Copyright (c) 2024 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

class UArticyImportData;
struct FArticyType;
class FString;
class CodeFileGenerator;

/**
 * Generates the ArticyLocalizer class from import data.
 */
class ArticyLocalizerGenerator
{
public:
	static void GenerateCode(const UArticyImportData* Data, FString& OutFile);

private:
	ArticyLocalizerGenerator() {}
	~ArticyLocalizerGenerator() {}
	static void IterateStringTables(CodeFileGenerator* Header, const FString& DirectoryPath, bool Indent = false);
	static void ModifyIniFile(const FString& IniFilePath, const FString& SectionName, const FString& KeyName, const FString& NewValue);
};
