//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyTypeGenerator.h"
#include "ArticyImportData.h"
#include "ArticyGlobalVariables.h"
#include "ArticyImporterHelpers.h"
#include "ArticyTypeSystem.h"
#include "CodeFileGenerator.h"
#include "CodeGenerator.h"

void ArticyTypeGenerator::GenerateCode(const UArticyImportData* Data, FString& OutFile)
{
	if(!ensure(Data))
		return;

	OutFile = CodeGenerator::GetArticyTypeClassname(Data, true);
	CodeFileGenerator(OutFile + ".h", true, [&](CodeFileGenerator* header)
	{
		header->Line("#include \"CoreUObject.h\"");
		header->Line("#include \"ArticyRuntime/Public/ArticyTypeSystem.h\"");
		header->Line("#include \"" + OutFile + ".generated.h\"");

		header->Line();

		// Generate the UArticyTypeSystem class
		const auto type = CodeGenerator::GetArticyTypeClassname(Data, false);
		header->Class(type + " : public UArticyTypeSystem", TEXT("Articy Type System"), true, [&]
		{
		});
	});
}

void ArticyTypeGenerator::GenerateAsset(const UArticyImportData* Data)
{
	const auto ClassName = CodeGenerator::GetArticyTypeClassname(Data, true);
	const auto AssetPackage = ArticyImporterHelpers::FindOrCreatePackage(ClassName);
	constexpr EObjectFlags Flags = RF_Public | RF_Standalone | RF_ArchetypeObject;

	if (const auto UClass = ArticyImporterHelpers::RetrieveClass(ClassName, FApp::GetProjectName(), UArticyTypeSystem::StaticClass()))
	{
		// If we successfully created the asset, load the type information and notify the asset registry
		if (UArticyTypeSystem* CreatedAsset = NewObject<UArticyTypeSystem>(AssetPackage, UClass, *FString(ClassName), Flags))
		{
			// Load type information
			CreatedAsset->Types.Reset();
			for(const auto type : Data->GetObjectDefs().GetTypes())
			{
				CreatedAsset->Types.Add(type.Key.ToString(), type.Value.ArticyType);
			}
		
			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(Cast<UObject>(CreatedAsset));

			// Mark the package dirty...
			AssetPackage->MarkPackageDirty();
		}
	}
}
