//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

class UArticyImportData;
struct FArticyGVInfo;

/**
 * Generates the project specific Articy Database class.
 */
class DatabaseGenerator
{
public:
	static void GenerateCode(const UArticyImportData* Data, FString& OutFile);
	static class UArticyDatabase* GenerateAsset(const UArticyImportData* Data);
};
