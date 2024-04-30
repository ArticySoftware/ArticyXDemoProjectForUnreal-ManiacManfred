//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "CoreMinimal.h"
#include "ArticyType.h"
#include "ArticyTextExtension.h"
#include "Internationalization/StringTableRegistry.h"
#include "UObject/UObjectIterator.h"
#include "UObject/Package.h"
#include "ArticyLocalizerSystem.generated.h"

UCLASS(BlueprintType)
class ARTICYRUNTIME_API UArticyLocalizerSystem : public UObject
{
	GENERATED_BODY()

public:
	static UArticyLocalizerSystem* Get()
	{
		static TWeakObjectPtr<UArticyLocalizerSystem> ArticyLocalizerSystem;
		if (ArticyLocalizerSystem.IsValid())
		{
			return ArticyLocalizerSystem.Get();
		}

		UClass* ParentClass = UArticyLocalizerSystem::StaticClass();

		// Iterate over all classes
		for (TObjectIterator<UClass> It; It; ++It)
		{
			UClass* Class = *It;
			// Check if the class is derived from the parent class
			if (Class->IsChildOf(ParentClass) && Class != ParentClass)
			{
				// Create an instance of the found subclass
				UArticyLocalizerSystem* NewLocalizerSystem = NewObject<UArticyLocalizerSystem>(GetTransientPackage(), Class);

				// Assign the instance to the TWeakObjectPtr
				ArticyLocalizerSystem = NewLocalizerSystem;

				return ArticyLocalizerSystem.Get();
			}
		}

		// Handle case where no subclass is found
		return nullptr;
	}

	virtual void Reload() {};

	inline FText ResolveText(UObject* Outer, const FText* SourceText)
	{
		return UArticyTextExtension::Get()->Resolve(Outer, SourceText);
	}

	inline FText LocalizeString(UObject* Outer, const FText& Key, bool ResolveTextExtension = true, const FText* BackupText = nullptr)
	{
		if (!bDataLoaded)
		{
			Reload();
		}

		const FText MissingEntry = FText::FromString("<MISSING STRING TABLE ENTRY>");

		// Look up entry in specified string table
		TOptional<FString> TableName = FTextInspector::GetNamespace(Key);
		if (!TableName.IsSet())
		{
			TableName = TEXT("ARTICY");
		}
		FText SourceString = FText::FromStringTable(
			FName(TableName.GetValue()),
			Key.ToString(),
			EStringTableLoadingPolicy::Find);
		if (!SourceString.IsEmpty() && !SourceString.EqualTo(MissingEntry))
		{
			if (ResolveTextExtension)
			{
				return ResolveText(Outer, &SourceString);
			}
			return SourceString;
		}

		if (BackupText)
		{
			return *BackupText;
		}

		// By default, return via the key
		if (ResolveTextExtension)
		{
			return ResolveText(Outer, &Key);
		}
		return Key;
	}

protected:
	bool bDataLoaded = false;
	bool bListenerSet = false;
};
