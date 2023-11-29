//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "ArticyObjectWith_Base.h"
#include "UObject/TextProperty.h"
#include "Engine/Engine.h"
#include "Internationalization/StringTable.h"
#include "ArticyAsset.h"
#include "ArticyTextExtension.h"
#include "ArticyObjectWithText.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UArticyObjectWithText : public UArticyObjectWith_Base { GENERATED_BODY() };

/**
 * All objects that have a property called 'Text' implement this interface.
 */
class IArticyObjectWithText : public IArticyObjectWith_Base
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="ArticyObjectWithText")
	virtual FText GetText()
	{
		static const auto PropName = FName("Text");
		FText& Key = GetProperty<FText>(PropName);
		const FText MissingEntry = FText::FromString("<MISSING STRING TABLE ENTRY>");

		// Look up entry in specified string table
		TOptional<FString> TableName = FTextInspector::GetNamespace(Key);
		if (!TableName.IsSet())
		{
			TableName = TEXT("ARTICY");
		}
		const FText SourceString = FText::FromStringTable(
			FName(TableName.GetValue()),
			Key.ToString(),
			EStringTableLoadingPolicy::FindOrFullyLoad);
		const FString Decoded = SourceString.ToString();
		if (!SourceString.IsEmpty() && !SourceString.EqualTo(MissingEntry))
		{
			return ResolveText(SourceString);
		}

		// By default, return via the key
		return ResolveText(Key);
	}

	virtual FText GetText() const
	{
		return const_cast<IArticyObjectWithText*>(this)->GetText();
	}
	
	//---------------------------------------------------------------------------//

	UFUNCTION(BlueprintCallable, Category="ArticyObjectWithText")
	virtual FText& SetText(UPARAM(ref) const FText& Text)
	{
		static const auto PropName = FName("Text");
		return GetProperty<FText>(PropName) = Text;
	}

	UFUNCTION(BlueprintCallable, Category = "ArticyObjectWithText")
	virtual USoundWave* GetVOAsset(UObject* WorldContext)
	{
		static const auto PropName = FName("Text");
		FText& Key = GetProperty<FText>(PropName);
		const FText MissingEntry = FText::FromString("<MISSING STRING TABLE ENTRY>");
		FArticyId AssetId;

		// Look up entry in specified string table
		TOptional<FString> TableName = FTextInspector::GetNamespace(Key);
		if (!TableName.IsSet())
		{
			TableName = TEXT("ARTICY");
		}
		const FText SourceString = FText::FromStringTable(
			FName(TableName.GetValue()),
			Key.ToString() + ".VOAsset",
			EStringTableLoadingPolicy::FindOrFullyLoad);
		const FString Decoded = SourceString.ToString();
		if (!SourceString.IsEmpty() && !SourceString.EqualTo(MissingEntry))
		{
			AssetId = FArticyId{ ResolveText(SourceString).ToString() };
		}
		else
		{
			AssetId = FArticyId{ ResolveText(FText::FromString(Key.ToString() + ".VOAsset")).ToString() };
		}

		const UArticyObject* AssetObject = UArticyObject::FindAsset(AssetId);
		if (AssetObject)
		{
			return (Cast<UArticyAsset>(AssetObject))->LoadAsSoundWave();
		}
		return nullptr;
	}

	virtual USoundWave* GetVOAsset() const
	{
		return const_cast<IArticyObjectWithText*>(this)->GetVOAsset();
	}

	//---------------------------------------------------------------------------//

protected:
	virtual FText ResolveText(FText SourceText) const
	{
		return UArticyTextExtension::Get()->Resolve(SourceText);
	}
};
