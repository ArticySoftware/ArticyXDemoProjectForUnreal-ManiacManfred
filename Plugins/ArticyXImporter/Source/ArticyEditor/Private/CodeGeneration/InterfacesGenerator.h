//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

class UArticyImportData;

class InterfacesGenerator
{

public:
	static void GenerateCode(const UArticyImportData* Data, FString& OutFile);

private:
	InterfacesGenerator() {};
	~InterfacesGenerator() {};
};
