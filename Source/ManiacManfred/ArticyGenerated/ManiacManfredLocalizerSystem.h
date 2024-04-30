﻿// articy Software GmbH & Co. KG
/** This code file was generated by ArticyImporter. Changes to this file will get lost once the code is regenerated. */

#pragma once

#include "CoreUObject.h"
#include "ArticyRuntime/Public/ArticyLocalizerSystem.h"
#include "ManiacManfredLocalizerSystem.generated.h"

/** Articy Localizer System */
UCLASS(BlueprintType)
class MANIACMANFRED_API UManiacManfredLocalizerSystem : public UArticyLocalizerSystem
{
	GENERATED_BODY()
	
public:
	void Reload() 
	{
		const FString& LangName = FInternationalization::Get().GetCurrentCulture()->GetTwoLetterISOLanguageName();
		if (!bListenerSet) {
			FInternationalization::Get().OnCultureChanged().AddUObject(this, &UManiacManfredLocalizerSystem::Reload);
			bListenerSet = true;
		}
FStringTableRegistry::Get().UnregisterStringTable(FName("Export_package"));
LOCTABLE_FROMFILE_GAME("Export_package", "Export_package", "ArticyContent/Generated/Export_package.csv");
		if (LangName.Equals("de")) {
			FStringTableRegistry::Get().UnregisterStringTable(FName("Export_package"));
			LOCTABLE_FROMFILE_GAME("Export_package", "Export_package", "L10N/de/ArticyContent/Generated/Export_package.csv");
		}
		if (LangName.Equals("en")) {
			FStringTableRegistry::Get().UnregisterStringTable(FName("Export_package"));
			LOCTABLE_FROMFILE_GAME("Export_package", "Export_package", "L10N/en/ArticyContent/Generated/Export_package.csv");
		}
		bDataLoaded = true;
	}
};