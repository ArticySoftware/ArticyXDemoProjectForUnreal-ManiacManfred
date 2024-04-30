//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "CoreMinimal.h"
#include "ArticyImportData.h"

enum EImportDataEnsureResult
{
	// found the import data in the asset registry
	AssetRegistry,
	// generated import data asset
	Generation,
	// the parameter was already valid
	Success,
	// didn't find the asset in asset registry nor was it generated
	Failure
};
class ARTICYEDITOR_API FArticyEditorFunctionLibrary
{
	
public:
	static int32 ForceCompleteReimport(UArticyImportData* = nullptr);
	static int32 ReimportChanges(UArticyImportData* = nullptr);
	static int32 RegenerateAssets(UArticyImportData* = nullptr);
	static EImportDataEnsureResult EnsureImportDataAsset(UArticyImportData**);

private:
	static UArticyImportData* GenerateImportDataAsset();
};
