//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

class UArticyImportData;
struct FArticyGVInfo;
class FString;

/**
 * Generates the ArticyGlobalVariables struct from import data.
 */
class GlobalVarsGenerator
{
public:
	static void GenerateCode(const UArticyImportData* Data, FString& OutFile);
	static void GenerateAsset(const UArticyImportData* Data);

private:
	GlobalVarsGenerator() {}
	~GlobalVarsGenerator() {}
};
