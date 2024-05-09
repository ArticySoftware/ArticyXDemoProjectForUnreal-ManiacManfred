//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ObjectDefinitionsGenerator.h"
#include "CodeFileGenerator.h"
#include "ObjectDefinitionsImport.h"
#include "ArticyImportData.h"

void ObjectDefinitionsGenerator::GenerateCode(const UArticyImportData* Data, FString& OutFile)
{
	OutFile = CodeGenerator::GetGeneratedTypesFilename(Data);
	CodeFileGenerator(OutFile + ".h", true, [&](CodeFileGenerator* header)
	{
		header->Line("#include \"CoreUObject.h\"");
		header->Line("#include \"ArticyBaseInclude.h\"");
		header->Line("#include \"" + CodeGenerator::GetGeneratedInterfacesFilename(Data) + ".h\"");
		header->Line("#include \"" + CodeGenerator::GetGeneratedTypesFilename(Data) + ".generated.h\"");
		header->Line();

		for(const auto type : Data->GetObjectDefs().GetTypes())
			type.Value.GenerateCode(*header, Data);
	});
}
