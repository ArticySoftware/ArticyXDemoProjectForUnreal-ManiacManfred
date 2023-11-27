//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "ArticyObjectWith_Base.h"
#include "ArticyObjectWithText.h"
#include "ArticyTextExtension.h"
#include "ArticyObjectWithMenuText.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UArticyObjectWithMenuText : public UArticyObjectWith_Base { GENERATED_BODY() };

/**
 * All objects that have a property called 'MenuText' implement this interface.
 */
class IArticyObjectWithMenuText : public IArticyObjectWith_Base
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category="ArticyObjectWithMenuText")
	virtual FText GetMenuText()
	{
		static const auto PropName = FName("MenuText");
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

		// By default, return ...
		return FText::FromString("...");
	}

	virtual const FText GetMenuText() const
	{
		return const_cast<IArticyObjectWithMenuText*>(this)->GetMenuText();
	}
	
	//---------------------------------------------------------------------------//

	UFUNCTION(BlueprintCallable, Category="ArticyObjectWithMenuText")
	virtual FText& SetMenuText(UPARAM(ref) const FText& MenuText)
	{
		static const auto PropName = FName("MenuText");
		return GetProperty<FText>(PropName) = MenuText;
	}

protected:
	virtual FText ResolveText(FText SourceText) const
	{
		return UArticyTextExtension::Get()->Resolve(SourceText);
	}
};
