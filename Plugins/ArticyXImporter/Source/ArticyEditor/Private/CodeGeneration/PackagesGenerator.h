//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

/**
 * Generates the package assets.
 */

class PackagesGenerator
{
public:

	static void GenerateAssets(class UArticyImportData* Data);

private:
	PackagesGenerator() {}
	virtual ~PackagesGenerator() {}
	
};
