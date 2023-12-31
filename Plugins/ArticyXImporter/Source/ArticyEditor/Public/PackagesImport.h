//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "ArticyAsset.h"
#include "ArticyBaseTypes.h"
#include "ArticyPackage.h"
#include "ArticyArchiveReader.h"
#include "ArticyTexts.h"

#include "PackagesImport.generated.h"


class UArticyImportData;
struct FADISettings;

USTRUCT(BlueprintType)
struct FArticyModelDef
{
	GENERATED_BODY()

public:
	
	void ImportFromJson(const TSharedPtr<FJsonObject> JsonModel);
	void GatherScripts(UArticyImportData* Data) const;
	UArticyObject* GenerateSubAsset(const UArticyImportData* Data, UObject* Outer) const;//MM_CHANGE


	FName GetType() const { return Type; }
	const FString& GetTechnicalName() const { return TechnicalName; }
	const FString& GetNameAndId() const { return NameAndId; }
	const FArticyId& GetId() const { return Id; }
	const FArticyId& GetParent() const { return Parent; }

	const FString& GetAssetRef() const { return AssetRef; }
	const EArticyAssetCategory& GetAssetCat() const { return AssetCategory; }
	TSharedPtr<FJsonObject> GetPropertiesJson() const;
	TSharedPtr<FJsonObject> GetTemplatesJson() const;

private:

	EArticyAssetCategory GetAssetCategoryFromString(const FString Category);

	/** The original type of the model as read in from json. */
	UPROPERTY(VisibleAnywhere, Category = "Model")
	FName Type;

	/** The TechnicalName of the model, extracted from the PropertiesJsonString. */
	UPROPERTY(VisibleAnywhere, Category = "Model")
	FString TechnicalName;
	/** The Id of the model, extracted from the PropertiesJsonString. */
	UPROPERTY(VisibleAnywhere, Category = "Model")
	FArticyId Id = -1;

	/** TechnicalName(Id) */
	UPROPERTY(VisibleAnywhere, Category = "Model")
	FString NameAndId;

	/** The Id of the parent of this model, extracted from the PropertiesJsonString. */
	UPROPERTY(VisibleAnywhere, Category = "Model")
	FArticyId Parent = 0;

	/** The asset reference, if this is an asset. */
	UPROPERTY(VisibleAnywhere, Category = "Model Meta")
	FString AssetRef;
	/** The asset category, if this is an asset. */
	UPROPERTY(VisibleAnywhere, Category = "Model Meta")
	EArticyAssetCategory AssetCategory = EArticyAssetCategory::None;

	UPROPERTY(VisibleAnywhere, Category = "Model")
	FString PropertiesJsonString;
	UPROPERTY(VisibleAnywhere, Category = "Model")
	FString TemplateJsonString;

	mutable TSharedPtr<FJsonObject> CachedPropertiesJson = nullptr;
	mutable TSharedPtr<FJsonObject> CachedTemplateJson = nullptr;
};

USTRUCT(BlueprintType)
struct FArticyPackageDef
{
	GENERATED_BODY()

public:

	void ImportFromJson(const UArticyArchiveReader& Archive, const TSharedPtr<FJsonObject>& JsonPackage);
	void GatherScripts(UArticyImportData* Data) const;
	void GatherText(const TSharedPtr<FJsonObject>& Json);
	UArticyPackage* GeneratePackageAsset(UArticyImportData* Data) const;//MM_CHANGE
	TMap<FString, FArticyTexts> GetTexts() const;

	FString GetFolder() const;
	FString GetFolderName() const;
	const FString GetName() const;
	const FString GetPreviousName() const;
	void SetName(const FString& NewName);
	FArticyId GetId() const;
	bool GetIsIncluded() const;
	FString GetScriptFragmentHash() const;

	bool operator==(const FArticyPackageDef& Other) const
	{
		return Id == Other.Id;
	}

private:

	UPROPERTY(VisibleAnywhere, Category = "Package")
	FArticyId Id;
	
	UPROPERTY(VisibleAnywhere, Category = "Package")
	FString Name;
	UPROPERTY(VisibleAnywhere, Category = "Package")
	FString Description;
	UPROPERTY(VisibleAnywhere, Category = "Package")
	bool IsDefaultPackage = false;

	UPROPERTY(VisibleAnywhere, Category = "Package")
	TArray<FArticyModelDef> Models;

	UPROPERTY(VisibleAnywhere, Category = "Package")
	TMap<FString, FArticyTexts> Texts;

	UPROPERTY(VisibleAnywhere, Category = "Package")
	FString PackageObjectsHash;

	UPROPERTY(VisibleAnywhere, Category = "Package")
	FString PackageTextsHash;

	UPROPERTY(VisibleAnywhere, Category = "Package")
	FString ScriptFragmentHash;

	bool IsIncluded = false;
	FString PreviousName = TEXT("");
};

/** Contains information about all imported packages. */
USTRUCT(BlueprintType)
struct FArticyPackageDefs
{
	GENERATED_BODY()

public:
	
	void ImportFromJson(const UArticyArchiveReader& Archive, const TArray<TSharedPtr<FJsonValue>>* Json, FADISettings& Settings);
	bool ValidateImport(const UArticyArchiveReader& Archive, const TArray<TSharedPtr<FJsonValue>>* Json);
	void GatherScripts(UArticyImportData* Data) const;
	void GenerateAssets(UArticyImportData* Data) const;//MM_CHANGE
	static TMap<FString, FArticyTexts> GetTexts(const FArticyPackageDef& Package);

	TSet<FString> GetPackageNames() const;
	TArray<FArticyPackageDef> GetPackages() const;
	void ResetPackages();
private:

	UPROPERTY(VisibleAnywhere, Category = "Packages")
	TArray<FArticyPackageDef> Packages;
};
