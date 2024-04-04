//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "ArticyImportData.h"

/**
 * 
 */
class ExpressoScriptsGenerator
{
public:
	static void GenerateCode(const UArticyImportData* Data, FString& OutFile);
	/** Returns the filename of the generated expresso scripts class (with extension). */
	static FString GetFilename(const UArticyImportData* Data);
};
