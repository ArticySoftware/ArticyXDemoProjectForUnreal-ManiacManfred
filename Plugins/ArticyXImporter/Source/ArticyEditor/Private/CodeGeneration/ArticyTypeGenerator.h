//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

class UArticyImportData;
struct FArticyType;

/**
 * Generates the ArticyType struct from import data.
 */
class ArticyTypeGenerator
{
public:
	static void GenerateCode(const UArticyImportData* Data, FString& OutFile);
	static void GenerateAsset(const UArticyImportData* Data);

private:
	ArticyTypeGenerator() {}
	~ArticyTypeGenerator() {}
};
